#include "TerrainGenerator.h"
#include "Chunk.h"
#include "core/Constants.h"

#include <cmath>
#include <random>

TerrainGenerator::TerrainGenerator(i32 seed)
    : m_seed(seed) {}

f32 TerrainGenerator::noise(f32 x, f32 z) const {
    // Simple noise — placeholder for Perlin/Simplex
    return std::sin(x * 0.1f + m_seed) * 0.5f +
           std::cos(z * 0.1f + m_seed * 2) * 0.3f +
           std::sin((x + z) * 0.05f) * 0.2f;
}

void TerrainGenerator::generate(Chunk* chunk) {
    i32 baseX = chunk->coord().x * CHUNK_SIZE_X;
    i32 baseZ = chunk->coord().z * CHUNK_SIZE_Z;

    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            f32 n = noise(static_cast<f32>(baseX + x),
                          static_cast<f32>(baseZ + z));
            i32 height = static_cast<i32>((n + 1.0f) * 0.5f * 20.0f) + 4;

            for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
                BlockID id = BLOCK_AIR;
                if (y == 0) {
                    id = BLOCK_BEDROCK;
                } else if (y < height - 1) {
                    id = BLOCK_STONE;
                } else if (y < height) {
                    id = BLOCK_DIRT;
                } else if (y == height) {
                    id = BLOCK_GRASS;
                }
                chunk->setBlock(x, y, z, id);
            }
        }
    }
}
