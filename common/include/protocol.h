#pragma once

#include <network.h>

class EchoPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

class LoginPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream &data) override;

    void serialize(const shared_any_ptr packet, std::ostream &out) override;
};

namespace PacketTypes {
    extern std::shared_ptr<EchoPacketType> ECHO;
    extern std::shared_ptr<LoginPacketType> LOGIN;
};

extern Protocol OPENVERSE_PROTOCOL;
