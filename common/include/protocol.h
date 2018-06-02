#pragma once

#include <network.h>

class EchoPacketType : public PacketType {
public:
    shared_any_ptr deserialize(std::istream& data) override {
        uint32_t val;
        buf_read(data, val);
        return std::make_shared(val);
    }

    void serialize(const shared_any_ptr packet, std::ostream& out) override {
        buf_write(out, std::static_pointer_cast<uint32_t>(packet));
    }
};

namespace PacketTypes {
    auto ECHO = std::make_shared<EchoPacketType>();
};


Protocol OPENVERSE_PROTOCOL {
        { ProtocolSide::EITHER, PacketTypes::ECHO },
};

