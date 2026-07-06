#pragma once

#include "core/Types.h"

class Chunk;

class TerrainGenerator {
public:
    TerrainGenerator(i32 seed = 42);
    void generate(Chunk* chunk);

private:
    f32 noise(f32 x, f32 z) const;
    i32 m_seed;
};
