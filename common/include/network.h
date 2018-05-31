#pragma once

#include <sstream>
#include <memory>
#include <vector>
#include <functional>

using PacketId = uint16_t;

using byte = unsigned char;

using shared_any_ptr = std::shared_ptr<void>;

using PacketHandler = std::function<void (shared_any_ptr)>;

// The last packet id
constexpr auto MAX_PACKET_ID = static_cast<PacketId>(-1);

// host to network functions, they're used to convert an integer from the host byte order
// to the network byte order (big endian).
// There are multiple definitions so that the
// template will know what's the best one to use

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

/**
 * A simple utility that converts any integer type in their respective network byte representation.
 * It automatically deals with endianness (converting host byte order to network byte order)
 */
template <typename T>
inline void buf_write(std::ostream& out, T val) {
    val = hton(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(T));
}


// network to host functions, they're used to convert an integer from the network byte order (big endian)
// to the host byte order.
// There are multiple definitions so that the
// template will know what's the best one to use

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

/**
 * A simple utility that converts any integer type in their respective host byte representation.
 * It automatically deals with endianness (converting network byte order to host byte order)
 */
template <typename T>
inline void buf_read(std::istream& in, T& val) {
    in.read(reinterpret_cast<char*>(&val), sizeof(T));
    val = ntoh(val);
}

/**
 * This class defines a type of packet, any instance will be used to describe a different type of packet.
 */
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

    virtual shared_any_ptr deserialize(std::istream& data) = 0;

    virtual void serialize(const shared_any_ptr packet, std::ostream& out) = 0;
};

/**
 * Used to indicate where a protocol will be used or where a packet will be sent/received.
 */
enum class ProtocolSide {
    CLIENT, SERVER, EITHER
};

/**
 * Intermediate immutable class that indicates every packet that the connection will use.
 * Every packet type has an associated ProtocolSide that describes where the packet will be sent.
 * With ProtocolSide::CLIENT only the client could send the packet (the inverse is assumed for SERVER).
 * With EITHER the same PacketType can be sent by both server and client connections.
 */
class Protocol {
public:
    const std::vector<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> packet_types;

    Protocol(const Protocol&) = delete;

    Protocol(std::initializer_list<std::pair<ProtocolSide, std::shared_ptr<PacketType>>> types);
};

/**
 * A class that describes a point to point connection
 */
class Connection {
protected:
    std::vector<PacketId> global_id_to_local_id_map;
    std::vector<std::pair<std::shared_ptr<PacketType>, PacketHandler>> receive_id_to_type_map;

    std::optional<PacketId> search_type_local_id(PacketType& type);

public:
    const ProtocolSide side;

    Connection(const Connection&) = delete;

    Connection(ProtocolSide side, const Protocol& protocol);

    std::tuple<std::shared_ptr<PacketType>, shared_any_ptr, PacketHandler> deserialize_packet(std::istream &data);

    void serialize_packet(PacketType& type, shared_any_ptr packet, std::ostream& out);

    void on_receive_packet(std::istream &data);

    virtual void send(PacketType& type, shared_any_ptr packet) = 0;

    void set_handler(PacketType& type, PacketHandler handler);
};

class Socket {
public:
    virtual const std::vector<std::shared_ptr<Connection>>& get_connections() = 0;
};

// Fake implementations

class FakeConnection : public Connection {
private:
    void on_receive_packet(PacketId local_id, shared_any_ptr& packet);
public:
    std::weak_ptr<FakeConnection> receiver{};

    FakeConnection(ProtocolSide side, const Protocol &protocol) : Connection(side, protocol){};

    void send(PacketType& type, shared_any_ptr packet) override;
};

class FakeSocket : public Socket {
    std::shared_ptr<FakeConnection> connection;
    std::vector<std::shared_ptr<Connection>> connections {connection};
public:
    FakeSocket(ProtocolSide side, const Protocol &protocol) {
        connection = std::make_shared<FakeConnection>(side, protocol);
    };

    const std::vector<std::shared_ptr<Connection>>& get_connections() override {
        return connections;
    }

    std::shared_ptr<FakeConnection>& get_connection() {
        return connection;
    }
};

std::pair<std::unique_ptr<FakeSocket>, std::unique_ptr<FakeSocket>> create_socket_pair(const Protocol& protocol);

