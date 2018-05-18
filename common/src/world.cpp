#include "world.h"

#define AIR_STATE nullptr // Todo: implement

// ------------------------------------------------------------------------ ArrayBlockStatePalette

ArrayBlockStatePalette::ArrayBlockStatePalette(unsigned bits, std::function<int(unsigned, BlockState *)> on_overflow) {
    this->id_length = bits;
    this->used = 0;
    this->array = std::make_unique<BlockState **>(new BlockState *[1 << bits]);
    // this->on_overflow = on_overflow;
}

ArrayBlockStatePalette::~ArrayBlockStatePalette() = default;

unsigned ArrayBlockStatePalette::get_id_length() {
    return id_length;
}

int ArrayBlockStatePalette::to_id(BlockState *block_state) {
    for (int i = 0; i < used; i++) {
        if ((*array)[i] == block_state) {
            return i;
        }
    }
    if (used < sizeof(array.get())) {
        (*array)[used] = block_state;
        return used++;
    } else {
        return on_overflow(id_length + 1, block_state);
    }
}

BlockState *ArrayBlockStatePalette::to_blockstate(unsigned int id) {
    return id >= 0 && id < sizeof(*array) ? (*array)[id] : nullptr;
}

// ------------------------------------------------------------------------ BlockStateStorage

BlockStateStorage::BlockStateStorage() {
    set_bits_per_palette(4);
}

BlockStateStorage::~BlockStateStorage() = default;

void BlockStateStorage::set_bits_per_palette(unsigned bits) {
    if (bits <= 4) {
        palette = std::make_unique<ArrayBlockStatePalette>(bits, [this](unsigned bits_needed, BlockState *overflowed) {
            VariableBitArray *old_ids = this->storage.get();
            BlockStatePalette *old_palette = this->palette.get();
            this->set_bits_per_palette(bits_needed);

            for (unsigned i = 0; i < old_ids->get_capacity(); i++) {
                BlockState *block_state = old_palette->to_blockstate(old_ids->get(i));
                if (block_state) {
                    this->set(i, block_state);
                }
            }

            return palette->to_id(overflowed);
        });
    } else if (bits <= 8) {
        palette = nullptr; // Todo: HashBlockStatePalette
    } else {
        palette = nullptr; // Todo: RegistryBlockStatePalette
    }
    palette->to_id(AIR_STATE);
    palette_bits = palette->get_id_length();
    storage = std::make_unique<VariableBitArray>(palette_bits, 16 * 16 * 16);
}

unsigned BlockStateStorage::get_index(unsigned x, unsigned y, unsigned z) {
    return y << 8u | z << 4u | x;
}

BlockState *BlockStateStorage::get(unsigned index) {
    return palette->to_blockstate(storage->get(index));
}

void BlockStateStorage::set(int index, BlockState *block_state) {
    storage->set(index, palette->to_id(block_state != nullptr ? block_state : AIR_STATE));
}

BlockState *BlockStateStorage::get(unsigned x, unsigned y, unsigned z) {
    if ((x & 15u) != x || (y & 15u) != y || (z & 15u) != z) {
        throw std::invalid_argument("invalid coordinates");
    }
    return get(get_index(x, y, z));
}

void BlockStateStorage::set(unsigned x, unsigned y, unsigned z, BlockState *blockstate) {
    if ((x & 15u) != x || (y & 15u) != y || (z & 15u) != z) {
        throw std::invalid_argument("invalid coordinates");
    }
    return set(get_index(x, y, z), blockstate);
}

// ------------------------------------------------------------------------ Chunk

Chunk::Chunk() = default;

Chunk::~Chunk() = default;

int Chunk::get_y() {
    return y;
}

BlockStateStorage *Chunk::get_blockstate_storage() {
    return &blockstate_storage;
}

BlockState *Chunk::get_blockstate(unsigned x, unsigned y, unsigned z) {
    return blockstate_storage.get(x, y, z);
}

void Chunk::set_blockstate(unsigned x, unsigned y, unsigned z, BlockState *blockstate) {
    blockstate_storage.set(x, y, z, blockstate);
}

// ------------------------------------------------------------------------ ChunkProvider

ChunkProvider::ChunkProvider() = default;

ChunkProvider::~ChunkProvider() = default;

Chunk *ChunkProvider::get(int y) {
    auto it = pillar.find(y);
    if (it == pillar.end()) {
        return nullptr;
    }
    return it->second;
}

void ChunkProvider::load(Chunk *chunk) {
    pillar.insert(std::make_pair(chunk->get_y(), chunk));
}

void ChunkProvider::unload(Chunk *chunk) {
    pillar.erase(chunk->get_y());
}

// ------------------------------------------------------------------------ ChunkPillar

ChunkPillar::ChunkPillar(int x, int z) {
    this->chunk_provider = std::make_unique<ChunkProvider>();
    this->x = x;
    this->z = z;
}

ChunkPillar::~ChunkPillar() = default;

int ChunkPillar::get_x() {
    return x;
}

int ChunkPillar::get_z() {
    return z;
}

ChunkProvider *ChunkPillar::get_chunk_provider() {
    return chunk_provider.get();
}

Chunk *ChunkPillar::get_chunk(int y) {
    return chunk_provider->get(y);
}

void ChunkPillar::load_chunk(Chunk *chunk) {
    chunk_provider->load(chunk);
}

void ChunkPillar::unload_chunk(Chunk *chunk) {
    chunk_provider->unload(chunk);
}

// ------------------------------------------------------------------------ ChunkPillarProvider

ChunkPillarProvider::ChunkPillarProvider() = default;

ChunkPillarProvider::~ChunkPillarProvider() = default;

unsigned long ChunkPillarProvider::get_index(int x, int z) {
    return ((unsigned long) x << 32ul) | ((unsigned long) z) & 0xffffffffful; // sign not needed
}

ChunkPillar *ChunkPillarProvider::get(int x, int z) {
    auto it = storage.find(get_index(x, z));
    if (it == storage.end()) {
        return nullptr;
    }
    return it->second;
}

void ChunkPillarProvider::load(ChunkPillar *chunkpillar) {
    storage.insert(std::pair(get_index(chunkpillar->get_x(), chunkpillar->get_z()), chunkpillar));
}

void ChunkPillarProvider::unload(int x, int z) {
    storage.erase(get_index(x, z));
}

// ------------------------------------------------------------------------ World

World::World(const std::string name) {
    this->name = name;
    this->chunkpillar_provider = std::make_unique<ChunkPillarProvider>();
}

World::~World() = default;

ChunkPillarProvider *World::get_chunkpillar_provider() {
    return chunkpillar_provider.get();
}

ChunkPillar *World::get_chunkpillar(int x, int z) {
    return chunkpillar_provider->get(x, z);
}

void World::load_chunkpillar(ChunkPillar *chunkpillar) {
    chunkpillar_provider->load(chunkpillar);
}

void World::unload_chunkpillar(int x, int z) {
    chunkpillar_provider->unload(x, z);
}
