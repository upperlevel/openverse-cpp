#pragma once

#include <sstream>
#include <memory>
#include <vector>
#include <functional>

using PacketId = uint16_t;

using byte = unsigned char;
using deleter_t = std::function<void(void *)>;
using unique_void_ptr = std::unique_ptr<void, deleter_t>;

constexpr auto MAX_PACKET_ID = static_cast<PacketId>(-1);

inline uint8_t hton(uint8_t val) {
    return val;
}

inline uint16_t hton(uint16_t val) {
    return htobe16(val);
}

inline uint32_t hton(uint32_t val) {
    return htobe32(val);
}

inline uint64_t hton(uint64_t val) {
    return htobe64(val);
}

template <typename T>
inline void buf_write(std::ostream& out, T val) {
    val = hton(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(T));
}

inline uint8_t ntoh(uint8_t val) {
    return val;
}

inline uint16_t ntoh(uint16_t val) {
    return be16toh(val);
}

inline uint32_t ntoh(uint32_t val) {
    return be32toh(val);
}

inline uint64_t ntoh(uint64_t val) {
    return be64toh(val);
}

template <typename T>
inline void buf_read(std::istream& in, T& val) {
    in.read(reinterpret_cast<char*>(&val), sizeof(T));
    val = ntoh(val);
}


class PacketType {
private:
    PacketId id = MAX_PACKET_ID;

public:
    PacketType(const PacketType&) = delete;
    PacketType() = default;

    void assign_id(PacketId id) {
        this->id = id;
    }

    PacketId get_id() const {
        return id;
    }

    virtual std::shared_ptr<void> deserialize(std::istream& data) = 0;

    virtual void serialize(const std::shared_ptr<void> packet, std::ostream& out) = 0;
};

enum class ProtocolSide {
    CLIENT, SERVER, EITHER
};


class Protocol {
public:
    const std::vector<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> packetTypes;

    Protocol(const Protocol&) = delete;

    Protocol(std::initializer_list<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> types);
};

class Connection {
private:
    std::vector<PacketId> global_id_to_local_id_map;
    std::vector<std::shared_ptr<PacketType>> receive_id_to_type_map;

public:
    const ProtocolSide side;

    Connection(const Connection&) = delete;

    Connection(ProtocolSide side, const Protocol& protocol);

    std::pair<std::shared_ptr<PacketType>, std::shared_ptr<void>> deserialize_packet(std::istream &data);

    void serialize_packet(PacketType& type, std::shared_ptr<void> packet, std::ostream& out);
};
