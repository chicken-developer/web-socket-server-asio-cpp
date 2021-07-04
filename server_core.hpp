//
// Created by dev on 7/5/21.
//

#ifndef GAME_MASTER_SERVER_SERVER_CORE_HPP
#define GAME_MASTER_SERVER_SERVER_CORE_HPP

#include "client_ws.hpp"
#include "server_ws.hpp"
#include <future>
#include "server_ws.hpp"

using WsServer = GameMasterServer::SocketServer<GameMasterServer::WS>;

namespace GameMasterServer {
    //Helper class, enum and struct
    enum class Event {
        LOGIN,
        REQUEST,
        INIT,
    };
    Event hash_event (std::string const& inString) {
        if (inString == "LOGIN") return Event::LOGIN;
        if (inString == "REQUEST") return Event::REQUEST;
    }

    struct Request {
        Event request;
        std::string requestParams;
    };

    class DataGenerator {
    public:
        DataGenerator()
        {

        }

        std::string GenerationPosition(std::string inputStr)
        {
            std::string position = "20_10";

            return position;
        }
        std::string GenerationPlayerData(std::string inputStr)
        {
            std::string data = "HP_100;MP_100";

            return data;
        }
    };
    //END Helper class, enum and struct

    class ServerRequestHandler {
    private:
        Request* request;

        std::string GetRequestStr(std::string inputStr, std::string delimiter)
        {
            //Need check valid and return default value if not valid
            return "LOGIN";
        }

        std::string GetParamsStr(std::string inputStr, std::string delimiter)
        {
            //Need check valid and return default value if not valid
            return "demomoo";

        }

    public:
        ServerRequestHandler()
        {
            request = new Request;
        };

        ~ServerRequestHandler()
        {
            delete request;
        }

        Request* GetRequest(const std::string inputStr)
        {
            std::cout << "GetRequest: Receive request: " << inputStr << std::endl;

            std::string delimiter = ":";
            std::string requestStr = GetRequestStr(inputStr, delimiter);
            std::string params = GetParamsStr(inputStr, delimiter);

            auto fullRequestWithParams = new Request{.request = hash_event(requestStr), .requestParams = params};

            return fullRequestWithParams;
        }

        std::string HandleRequest(Request* requestInStock)
        {
            std::cout << "HandleRequest: Message received: " << requestInStock->requestParams << std::endl;
            std::string output = "Demoxxx";
            return output;
        }


    };


    class GameServerEntryPoint {
    private:
        ServerRequestHandler* requestHandler;
        ~ GameServerEntryPoint() {
            delete requestHandler;
        }

    public:
        GameServerEntryPoint() {
            requestHandler = new ServerRequestHandler();
        }

        std::string HandleInput(std::shared_ptr<GameMasterServer::SocketServerBase<boost::asio::basic_stream_socket<boost::asio::ip::tcp>>::InMessage> in_message) {
            std::string inputStr = in_message->string();
            std::cout << "HandleInput: Message received: " << inputStr << std::endl;

            auto event = requestHandler->GetRequest(inputStr);
            std::string output = requestHandler->HandleRequest(event);
            return output;
        }

    };
}


#endif //GAME_MASTER_SERVER_SERVER_CORE_HPP
