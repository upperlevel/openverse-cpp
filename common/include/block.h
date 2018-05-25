#pragma once

#include <string>

class BlockType;
class BlockState;

class BlockType {
private:
    const std::string id;
    int rawId;

public:
    explicit BlockType(const std::string id);
    ~BlockType();

    void set_default_block_state(BlockState *state);
    BlockState *get_block_state(int meta);

};

class BlockState {

};
