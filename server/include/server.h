#pragma once

#include "network.h"

namespace Openverse {
    class Server {
    private:
        std::unique_ptr<Socket> socket;

    public:
        /**
         * Creates the server with the socket it will use.
         */
        Server(Socket& socket);

        /**
         * Loads resources and starts listening for incoming connections.
         */
        void start();
        void stop();
    };
}
