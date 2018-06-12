#include "server.h"
#include "protocol.h"

#define DEBUG
#include <iostream>

Openverse::Server::Server(Socket& socket) {
    this->socket = &socket;
}

void Openverse::Server::start() {
    // Todo load resources
    // Todo start listening for incoming clients

    // Login listener
    std::string login_packet;
    socket->set_handler(*PacketTypes::LOGIN, [&login_packet](auto &connection, auto packet) -> void {
#ifdef DEBUG
        std::cout << "Player logged in: " << login_packet << std::endl;
#endif
    });
}
