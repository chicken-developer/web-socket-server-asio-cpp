//
// Created by dev on 5/9/21.
//

#ifndef GAME_MASTERSOCKET_SERVER_GAME_SERVER_ENDPOINT_HPP
#define GAME_MASTERSOCKET_SERVER_GAME_SERVER_ENDPOINT_HPP

#include "client_ws.hpp"
#include "server_ws.hpp"
#include <future>
#include "server_ws.hpp"
#include "game_server_endpoint.hpp"
using WsServer = GameMasterServer::SocketServer<GameMasterServer::WS>;

namespace GameMasterServer {
    class GameLogicEntryPointHandler {
    public :
        GameLogicEntryPointHandler() = default {

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

    class GameServerEntryPointHandler {
    public:
        GameServerEntryPointHandler() {
            logicHandler = new GameLogicEntryPointHandler();
        }

        std::string HandleInput(std::shared_ptr<GameMasterServer::SocketServerBase<boost::asio::basic_stream_socket<boost::asio::ip::tcp>>::InMessage> in_message) {
            std::string inputStr = in_message->string();

            switch(inputStr)
            {
                case "LOGIN:_$userName" => userName
            }

            std::string output = inputStr + "demo";

            return output;
        }

    private:
        GameLogicEntryPointHandler *logicHandler;
        std::string Login(std::string userName)
        {
            std::string dataForPlay = logicHandler.GenerationPosition(userName);

            return dataForPlay;
        }
    };
}



#endif //GAME_MASTERSOCKET_SERVER_GAME_SERVER_ENDPOINT_HPP
