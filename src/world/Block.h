#pragma once

#include "core/Types.h"
#include "core/Constants.h"

struct BlockData {
    BlockID id = BLOCK_AIR;
    bool transparent = false;
    bool solid = true;
    f32 hardness = 1.0f;
};

class Block {
public:
    static const BlockData& get(BlockID id);
};
