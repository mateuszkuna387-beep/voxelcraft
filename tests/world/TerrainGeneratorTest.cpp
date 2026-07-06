#include <gtest/gtest.h>

#include "world/TerrainGenerator.h"
#include "world/Chunk.h"
#include "core/Constants.h"

TEST(TerrainGeneratorTest, GeneratesBlocks) {
    TerrainGenerator gen(42);
    Chunk chunk(0, 0);
    gen.generate(&chunk);

    EXPECT_FALSE(chunk.isEmpty());
}

TEST(TerrainGeneratorTest, BedrockAtBottom) {
    TerrainGenerator gen(42);
    Chunk chunk(0, 0);
    gen.generate(&chunk);

    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            EXPECT_EQ(chunk.getBlock(x, 0, z), BLOCK_BEDROCK);
        }
    }
}

TEST(TerrainGeneratorTest, DeterministicWithSeed) {
    TerrainGenerator gen1(42);
    TerrainGenerator gen2(42);
    Chunk c1(0, 0), c2(0, 0);

    gen1.generate(&c1);
    gen2.generate(&c2);

    for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                EXPECT_EQ(c1.getBlock(x, y, z), c2.getBlock(x, y, z));
            }
        }
    }
}
