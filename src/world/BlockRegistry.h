#pragma once

#include "core/Types.h"

class BlockRegistry {
public:
    static BlockID getID(const char* name);
    static const char* getName(BlockID id);
};
