#include "block.h"

#include <math.h>

BlockType::BlockType(const char *id) {
    this->id = id;
}

BlockType::~BlockType() = default;
