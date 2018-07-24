#include "chunk.h"

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
