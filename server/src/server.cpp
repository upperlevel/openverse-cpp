#include "server.h"
#include "protocol.h"

#define DEBUG
#include <iostream>

Openverse::Server::Server(Socket& socket) : socket(std::unique_ptr<Socket>(&socket)) {}

void Openverse::Server::start() {
    // Todo load resources
    // Todo start listening for incoming clients

    // Login listener
    std::string login_packet;
    socket->set_handler(*PacketTypes::LOGIN, [&](auto &connection, auto packet) {
        // registers the player
        std::string player_name = *std::static_pointer_cast<std::string>(packet);
        std::shared_ptr<Player> player(new Player(connection, player_name));
        this->players.push_back(player);
        std::cout << "Server: new player added: " << player->get_name() << std::endl;

        // sends the name of the world so the player can start requesting
        std::cout << "Server: PlayerChangeWorldPacket sent to player: "  << player->get_name() << std::endl;
        player->send_packet(*PacketTypes::PLAYER_CHANGE_WORLD, std::make_shared<std::string>("todo_world_name"));
    });
}
