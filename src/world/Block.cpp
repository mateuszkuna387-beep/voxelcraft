#include "Block.h"
#include "core/Constants.h"

#include <unordered_map>

static const std::unordered_map<BlockID, BlockData> s_blockRegistry = {
    { BLOCK_AIR,     { BLOCK_AIR,     true,  false, 0.0f,  glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) } },
    { BLOCK_STONE,   { BLOCK_STONE,   false, true,  2.0f,  glm::vec3(0.50f, 0.50f, 0.52f), glm::vec3(0.50f, 0.50f, 0.52f), glm::vec3(0.50f, 0.50f, 0.52f) } },
    { BLOCK_DIRT,    { BLOCK_DIRT,    false, true,  0.5f,  glm::vec3(0.55f, 0.33f, 0.14f), glm::vec3(0.55f, 0.33f, 0.14f), glm::vec3(0.55f, 0.33f, 0.14f) } },
    { BLOCK_GRASS,   { BLOCK_GRASS,   false, true,  0.6f,  glm::vec3(0.25f, 0.62f, 0.18f), glm::vec3(0.38f, 0.28f, 0.10f), glm::vec3(0.55f, 0.33f, 0.14f) } },
    { BLOCK_WOOD,    { BLOCK_WOOD,    false, true,  1.5f,  glm::vec3(0.65f, 0.45f, 0.20f), glm::vec3(0.40f, 0.26f, 0.13f), glm::vec3(0.65f, 0.45f, 0.20f) } },
    { BLOCK_LEAVES,  { BLOCK_LEAVES,  true,  true,  0.3f,  glm::vec3(0.18f, 0.55f, 0.18f), glm::vec3(0.18f, 0.55f, 0.18f), glm::vec3(0.18f, 0.55f, 0.18f) } },
    { BLOCK_SAND,    { BLOCK_SAND,    false, true,  0.5f,  glm::vec3(0.76f, 0.70f, 0.50f), glm::vec3(0.76f, 0.70f, 0.50f), glm::vec3(0.76f, 0.70f, 0.50f) } },
    { BLOCK_WATER,   { BLOCK_WATER,   true,  false, 0.0f,  glm::vec3(0.20f, 0.40f, 0.80f), glm::vec3(0.20f, 0.40f, 0.80f), glm::vec3(0.20f, 0.40f, 0.80f) } },
    { BLOCK_BEDROCK, { BLOCK_BEDROCK, false, true,  -1.0f, glm::vec3(0.30f, 0.30f, 0.30f), glm::vec3(0.30f, 0.30f, 0.30f), glm::vec3(0.30f, 0.30f, 0.30f) } },
};

const BlockData& Block::get(BlockID id) {
    static const BlockData s_default{ BLOCK_AIR, true, false, 0.0f };
    auto it = s_blockRegistry.find(id);
    if (it != s_blockRegistry.end()) {
        return it->second;
    }
    return s_default;
}
