#pragma once

#include <glm/glm.hpp>

#include "core/Types.h"
#include "core/Constants.h"

struct BlockData {
    BlockID id = BLOCK_AIR;
    bool transparent = false;
    bool solid = true;
    f32 hardness = 1.0f;
    glm::vec3 topColor{ 0.5f };
    glm::vec3 sideColor{ 0.5f };
    glm::vec3 bottomColor{ 0.5f };
};

class Block {
public:
    static const BlockData& get(BlockID id);
};
