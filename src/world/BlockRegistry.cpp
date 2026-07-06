#include "BlockRegistry.h"
#include "core/Constants.h"

#include <cstring>
#include <unordered_map>

static const std::unordered_map<BlockID, const char*> s_idToName = {
    { BLOCK_AIR,     "air" },
    { BLOCK_STONE,   "stone" },
    { BLOCK_DIRT,    "dirt" },
    { BLOCK_GRASS,   "grass" },
    { BLOCK_WOOD,    "wood" },
    { BLOCK_LEAVES,  "leaves" },
    { BLOCK_SAND,    "sand" },
    { BLOCK_WATER,   "water" },
    { BLOCK_BEDROCK, "bedrock" },
};

BlockID BlockRegistry::getID(const char* name) {
    for (const auto& [id, n] : s_idToName) {
        if (strcmp(n, name) == 0) return id;
    }
    return BLOCK_AIR;
}

const char* BlockRegistry::getName(BlockID id) {
    auto it = s_idToName.find(id);
    return it != s_idToName.end() ? it->second : "unknown";
}
