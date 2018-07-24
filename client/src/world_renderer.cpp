#include <protocol.h>
#include "world_renderer.h"

// ------------------------------------------------------------------------------------------------ WorldPacketListener

WorldPacketListener::WorldPacketListener(World &world, Socket &socket) {
    socket.set_handler(*PacketTypes::CHUNK_CREATE, [&](auto connection, auto packet) {
        // Gets the chunk pillar where the chunk has to be added.
        // Then init a chunk instance and add the chunk.
        std::shared_ptr<ChunkCreatePacket> pkt = std::static_pointer_cast<ChunkCreatePacket>(packet);
        ChunkPillar& chk_plr = world.get_chunk_pillar(pkt->chunk_x, pkt->chunk_z);
        chk_plr.set_chunk()
    });

    socket.set_handler(*Packet)
}
