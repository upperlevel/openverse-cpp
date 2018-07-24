#include "world.h"

// ------------------------------------------------------------------------------------------------ World

World::World(std::string name) : name(name) {}

const std::string& World::get_name() {
    return name;
}

std::weak_ptr<ChunkPillar> World::get_chunk_pillar(int x, int z) {
    unsigned long i = ((unsigned long) x << 32ul) | ((unsigned long) z) & 0xffffffffful;
    auto got = storage.find(i);
    if (got == storage.end()) {
        // Currently if the asked ChunkPillar hasn't already been generated
        // generates it.
        std::shared_ptr<ChunkPillar> new_one = std::make_shared(x, z);
        storage.insert(std::pair(i, new_one));
        return new_one;
    }
    return got->second;
}
