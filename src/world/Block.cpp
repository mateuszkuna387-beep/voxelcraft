#include "Block.h"
#include "core/Constants.h"

#include <unordered_map>

static const std::unordered_map<BlockID, BlockData> s_blockRegistry = {
    { BLOCK_AIR,     { BLOCK_AIR,     true,  false, 0.0f } },
    { BLOCK_STONE,   { BLOCK_STONE,   false, true,  2.0f } },
    { BLOCK_DIRT,    { BLOCK_DIRT,    false, true,  0.5f } },
    { BLOCK_GRASS,   { BLOCK_GRASS,   false, true,  0.6f } },
    { BLOCK_WOOD,    { BLOCK_WOOD,    false, true,  1.5f } },
    { BLOCK_LEAVES,  { BLOCK_LEAVES,  true,  true,  0.3f } },
    { BLOCK_SAND,    { BLOCK_SAND,    false, true,  0.5f } },
    { BLOCK_WATER,   { BLOCK_WATER,   true,  false, 0.0f } },
    { BLOCK_BEDROCK, { BLOCK_BEDROCK, false, true,  -1.0f } },
};

const BlockData& Block::get(BlockID id) {
    static const BlockData s_default{ BLOCK_AIR, true, false, 0.0f };
    auto it = s_blockRegistry.find(id);
    if (it != s_blockRegistry.end()) {
        return it->second;
    }
    return s_default;
}
