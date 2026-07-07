#include <gtest/gtest.h>

#include "world/TerrainGenerator.h"
#include "world/Chunk.h"
#include "world/World.h"
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

TEST(TerrainGeneratorTest, SurfaceHasGrass) {
    TerrainGenerator gen(42);
    Chunk chunk(0, 0);
    gen.generate(&chunk);

    bool foundGrass = false;
    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 y = 1; y < CHUNK_SIZE_Y; ++y) {
                BlockID id = chunk.getBlock(x, y, z);
                if (id == BLOCK_GRASS) {
                    foundGrass = true;
                    EXPECT_NEAR(static_cast<f32>(y), 35.0f, 32.0f);
                    break;
                }
            }
        }
    }
    EXPECT_TRUE(foundGrass);
}

TEST(TerrainGeneratorTest, HasCaves) {
    TerrainGenerator gen(42);
    Chunk chunk(0, 0);
    gen.generate(&chunk);

    bool hasStone = false;
    bool hasAirBelowSurface = false;
    i32 maxY = 0;
    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
                BlockID id = chunk.getBlock(x, y, z);
                if (id == BLOCK_GRASS) {
                    if (y > maxY) maxY = y;
                }
                if (id == BLOCK_STONE) hasStone = true;
            }
        }
    }

    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 y = 3; y < maxY - 3 && y < WORLD_SIZE_Y; ++y) {
                BlockID id = chunk.getBlock(x, y, z);
                if (id == BLOCK_AIR && y > 1 && y < maxY - 3) {
                    hasAirBelowSurface = true;
                    goto foundCave;
                }
            }
        }
    }
    foundCave:
    EXPECT_TRUE(hasStone);
    EXPECT_TRUE(hasAirBelowSurface);
}

TEST(TerrainGeneratorTest, HeightVariation) {
    TerrainGenerator gen(42);
    i32 minH = 100, maxH = 0;

    for (i32 cx = 0; cx < 3; ++cx) {
        for (i32 cz = 0; cz < 3; ++cz) {
            Chunk chunk(cx, cz);
            gen.generate(&chunk);

            for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
                    for (i32 y = 1; y < CHUNK_SIZE_Y; ++y) {
                        if (chunk.getBlock(x, y, z) == BLOCK_GRASS) {
                            if (y < minH) minH = y;
                            if (y > maxH) maxH = y;
                            break;
                        }
                    }
                }
            }
        }
    }

    EXPECT_GT(maxH - minH, 5);
}

TEST(TerrainGeneratorTest, WorldBounds) {
    EXPECT_TRUE(World::inBounds(0, 0, 0));
    EXPECT_TRUE(World::inBounds(99, 99, 99));
    EXPECT_FALSE(World::inBounds(-1, 0, 0));
    EXPECT_FALSE(World::inBounds(0, -1, 0));
    EXPECT_FALSE(World::inBounds(0, 0, -1));
    EXPECT_FALSE(World::inBounds(100, 0, 0));
    EXPECT_FALSE(World::inBounds(0, 100, 0));
    EXPECT_FALSE(World::inBounds(0, 0, 100));
}
