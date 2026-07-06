#include <gtest/gtest.h>

#include "world/Chunk.h"
#include "core/Constants.h"

TEST(ChunkTest, NewChunkIsEmpty) {
    Chunk chunk(0, 0);
    EXPECT_TRUE(chunk.isEmpty());
}

TEST(ChunkTest, SetAndGetBlock) {
    Chunk chunk(0, 0);
    chunk.setBlock(1, 2, 3, BLOCK_STONE);
    EXPECT_EQ(chunk.getBlock(1, 2, 3), BLOCK_STONE);
}

TEST(ChunkTest, OutOfBoundsReturnsAir) {
    Chunk chunk(0, 0);
    EXPECT_EQ(chunk.getBlock(-1, 0, 0), BLOCK_AIR);
    EXPECT_EQ(chunk.getBlock(0, -1, 0), BLOCK_AIR);
    EXPECT_EQ(chunk.getBlock(0, 0, -1), BLOCK_AIR);
    EXPECT_EQ(chunk.getBlock(CHUNK_SIZE_X, 0, 0), BLOCK_AIR);
    EXPECT_EQ(chunk.getBlock(0, CHUNK_SIZE_Y, 0), BLOCK_AIR);
    EXPECT_EQ(chunk.getBlock(0, 0, CHUNK_SIZE_Z), BLOCK_AIR);
}

TEST(ChunkTest, NotEmptyAfterSettingBlock) {
    Chunk chunk(0, 0);
    chunk.setBlock(0, 0, 0, BLOCK_DIRT);
    EXPECT_FALSE(chunk.isEmpty());
}

TEST(ChunkTest, ChunkCoords) {
    Chunk chunk(3, -2);
    EXPECT_EQ(chunk.coord().x, 3);
    EXPECT_EQ(chunk.coord().z, -2);
}
