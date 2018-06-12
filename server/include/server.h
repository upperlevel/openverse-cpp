#pragma once

#include "network.h"

namespace Openverse {
    class Server {
    private:
        Socket& socket;

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
