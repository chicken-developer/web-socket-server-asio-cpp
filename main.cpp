#include "client_ws.hpp"
#include "server_ws.hpp"
#include "server_core.hpp"
#include <future>

using namespace std;
using namespace GameMasterServer;

using WsServer = GameMasterServer::SocketServer<GameMasterServer::WS>;
using WsClient = GameMasterServer::SocketClient<GameMasterServer::WS>;

int main() {
    WsServer server;
    server.config.port = 8432;
    server.config.address = "192.168.220.129";

    auto &echo = server.endpoint["^/echo/?$"];

    echo.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
        //auto out_message = in_message->string();
        GameServerEntryPoint static messageHandler;
        auto out_message = messageHandler.HandleInput(in_message);
        cout << "Server: Message received: \"" << out_message << "\" from " << connection.get() << endl;
        cout << "Server: Sending message \"" << out_message << "\" to " << connection.get() << endl;
        // connection->send is an asynchronous function
        connection->send(out_message, [](const GameMasterServer::error_code &ec) {
            if(ec) {
                cout << "Server: Error sending message. " <<
                     // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                     "Error: " << ec << ", error message: " << ec.message() << endl;
            }
        });
    };

    echo.on_open = [](shared_ptr<WsServer::Connection> connection) {
        cout << "Server: Opened connection " << connection.get() << endl;
    };

    echo.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
        cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
    };

    echo.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, GameMasterServer::CaseInsensitiveMultimap & /*response_header*/) {
        return GameMasterServer::StatusCode::information_switching_protocols; // Upgrade to websocket
    };

    echo.on_error = [](shared_ptr<WsServer::Connection> connection, const GameMasterServer::error_code &ec) {
        cout << "Server: Error in connection " << connection.get() << ". "
             << "Error: " << ec << ", error message: " << ec.message() << endl;
    };

    auto &echo_thrice = server.endpoint["^/echo_thrice/?$"];
    echo_thrice.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
        auto out_message = make_shared<string>(in_message->string());

        connection->send(*out_message, [connection, out_message](const GameMasterServer::error_code &ec) {
            if(!ec)
                connection->send(*out_message); // Sent after the first send operation is finished
        });
        connection->send(*out_message); // Most likely queued. Sent after the first send operation is finished.
    };

    auto &echo_all = server.endpoint["^/echo_all/?$"];
    echo_all.on_message = [&server](shared_ptr<WsServer::Connection> /*connection*/, shared_ptr<WsServer::InMessage> in_message) {
        GameServerEntryPoint static messageHandler;
        auto out_message = messageHandler.HandleInput(in_message);

        // echo_all.get_connections() can also be used to solely receive connections on this endpoint
        for(auto &a_connection : server.get_connections())
            a_connection->send(out_message);
    };

    // Start server and receive assigned port when server is listening for requests
    promise<unsigned short> server_port;
    thread server_thread([&server, &server_port]() {
        // Start server
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    cout << "Server listening on port " << server_port.get_future().get() << endl
         << endl;

    server_thread.join();
}
