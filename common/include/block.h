#pragma once

#include <string>

class BlockType;
class BlockState;

enum class BlockFace {
    UP, DOWN, FRONT, BACK, RIGHT, LEFT;
};

class BlockType {
private:
    int raw_id;

public:
    const std::string id;

    explicit BlockType(const std::string id);
    ~BlockType();

    void set_default_block_state(BlockState *state);
    BlockState *get_block_state(int meta);

};

class BlockState {

};
