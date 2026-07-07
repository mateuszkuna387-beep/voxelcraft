#include "TerrainGenerator.h"
#include "Chunk.h"
#include "core/Constants.h"

#include <cmath>
#include <random>

TerrainGenerator::TerrainGenerator(i32 seed)
    : m_seed(seed) {}

f32 TerrainGenerator::hash(i32 x, i32 y) const {
    i32 h = m_seed + x * 374761393 + y * 668265263;
    h = (h ^ (h >> 13)) * 1274126177;
    h ^= h >> 16;
    return static_cast<f32>(h & 0x7FFFFFFF) / 2147483648.0f;
}

f32 TerrainGenerator::hash3D(i32 x, i32 y, i32 z) const {
    i32 h = m_seed + x * 374761393 + y * 668265263 + z * 1274126177;
    h = (h ^ (h >> 13)) * 1274126177;
    h ^= h >> 16;
    return static_cast<f32>(h & 0x7FFFFFFF) / 2147483648.0f;
}

f32 TerrainGenerator::smoothNoise2D(f32 x, f32 y, i32 octaves) const {
    f32 value = 0.0f;
    f32 amplitude = 1.0f;
    f32 frequency = 1.0f;
    f32 maxValue = 0.0f;

    for (i32 o = 0; o < octaves; ++o) {
        i32 ix = static_cast<i32>(std::floor(x * frequency));
        i32 iy = static_cast<i32>(std::floor(y * frequency));
        f32 fx = x * frequency - ix;
        f32 fy = y * frequency - iy;

        fx = fx * fx * (3.0f - 2.0f * fx);
        fy = fy * fy * (3.0f - 2.0f * fy);

        f32 v00 = hash(ix, iy);
        f32 v10 = hash(ix + 1, iy);
        f32 v01 = hash(ix, iy + 1);
        f32 v11 = hash(ix + 1, iy + 1);

        f32 v = (v00 * (1.0f - fx) + v10 * fx) * (1.0f - fy) +
                (v01 * (1.0f - fx) + v11 * fx) * fy;

        value += v * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return value / maxValue;
}

f32 TerrainGenerator::smoothNoise3D(f32 x, f32 y, f32 z) const {
    i32 ix = static_cast<i32>(std::floor(x));
    i32 iy = static_cast<i32>(std::floor(y));
    i32 iz = static_cast<i32>(std::floor(z));
    f32 fx = x - ix;
    f32 fy = y - iy;
    f32 fz = z - iz;

    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);
    fz = fz * fz * (3.0f - 2.0f * fz);

    f32 v000 = hash3D(ix, iy, iz);
    f32 v100 = hash3D(ix + 1, iy, iz);
    f32 v010 = hash3D(ix, iy + 1, iz);
    f32 v110 = hash3D(ix + 1, iy + 1, iz);
    f32 v001 = hash3D(ix, iy, iz + 1);
    f32 v101 = hash3D(ix + 1, iy, iz + 1);
    f32 v011 = hash3D(ix, iy + 1, iz + 1);
    f32 v111 = hash3D(ix + 1, iy + 1, iz + 1);

    f32 v00 = v000 * (1.0f - fx) + v100 * fx;
    f32 v10 = v010 * (1.0f - fx) + v110 * fx;
    f32 v01 = v001 * (1.0f - fx) + v101 * fx;
    f32 v11 = v011 * (1.0f - fx) + v111 * fx;

    f32 v0 = v00 * (1.0f - fy) + v10 * fy;
    f32 v1 = v01 * (1.0f - fy) + v11 * fy;

    return v0 * (1.0f - fz) + v1 * fz;
}

void TerrainGenerator::generate(Chunk* chunk) {
    i32 baseX = chunk->coord().x * CHUNK_SIZE_X;
    i32 baseZ = chunk->coord().z * CHUNK_SIZE_Z;

    if (baseX >= WORLD_SIZE_X || baseZ >= WORLD_SIZE_Z) return;
    if (baseX + CHUNK_SIZE_X <= 0 || baseZ + CHUNK_SIZE_Z <= 0) return;

    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        i32 gx = baseX + x;
        if (gx < 0 || gx >= WORLD_SIZE_X) continue;

        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            i32 gz = baseZ + z;
            if (gz < 0 || gz >= WORLD_SIZE_Z) continue;

            f32 n = smoothNoise2D(static_cast<f32>(gx) * 0.025f,
                                  static_cast<f32>(gz) * 0.025f, 4);
            i32 height = static_cast<i32>(n * 30.0f + 35.0f);
            if (height < 2) height = 2;
            if (height >= WORLD_SIZE_Y) height = WORLD_SIZE_Y - 1;

            for (i32 y = 0; y < WORLD_SIZE_Y; ++y) {
                BlockID id = BLOCK_AIR;

                if (y == 0) {
                    id = BLOCK_BEDROCK;
                } else if (y == height) {
                    id = BLOCK_GRASS;
                } else if (y < height - 3) {
                    id = BLOCK_STONE;
                } else if (y < height) {
                    id = BLOCK_DIRT;
                }

                if (id == BLOCK_STONE && y > 1 && y < height - 3) {
                    f32 caveNoise = smoothNoise3D(
                        static_cast<f32>(gx) * 0.08f,
                        static_cast<f32>(y) * 0.08f,
                        static_cast<f32>(gz) * 0.08f);
                    f32 depthFactor = static_cast<f32>(height - y) / 40.0f;
                    f32 threshold = 0.32f + depthFactor * 0.15f;

                    if (caveNoise < threshold) {
                        id = BLOCK_AIR;
                    }
                }

                if (id == BLOCK_STONE && y >= height - 5 && y < height - 1) {
                    f32 entranceNoise = smoothNoise3D(
                        static_cast<f32>(gx) * 0.12f,
                        static_cast<f32>(y) * 0.12f,
                        static_cast<f32>(gz) * 0.12f);
                    if (entranceNoise < 0.15f) {
                        id = BLOCK_AIR;
                    }
                }

                chunk->setBlock(x, y, z, id);
            }
        }
    }
}
