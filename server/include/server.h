#pragma once

#include "network.h"
#include "player.h"

namespace Openverse {
    class Server {
    private:
        std::unique_ptr<Socket> socket;
        std::vector<std::shared_ptr<Player>> players;

    public:
        /**
         * Creates the server with the socket it will use.
         */
        Server(Socket& socket);

        /**
         * Loads resources and starts listening for incoming connections.
         */
        void start();
    };
}
