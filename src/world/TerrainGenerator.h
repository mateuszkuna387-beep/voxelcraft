#pragma once

#include "core/Types.h"

class Chunk;

class TerrainGenerator {
public:
    TerrainGenerator(i32 seed = 42);
    void generate(Chunk* chunk);

private:
    f32 hash(i32 x, i32 y) const;
    f32 hash3D(i32 x, i32 y, i32 z) const;
    f32 smoothNoise2D(f32 x, f32 y, i32 octaves = 4) const;
    f32 smoothNoise3D(f32 x, f32 y, f32 z) const;

    i32 m_seed;
};
