#include "player.h"

Player::Player(Connection& connection , std::string name) : connection(std::unique_ptr<Connection>(&connection)), name(name) {}

const Connection& Player::get_connection() {
    return *connection;
}

const std::string& Player::get_name() {
    return name;
}

void Player::send_packet(PacketType &packet_type, shared_any_ptr packet_data) {
    connection->send(packet_type, packet_data);
}
