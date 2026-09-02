#include <gtest/gtest.h>

#include "world/Block.h"
#include "world/Chunk.h"
#include "core/Constants.h"

TEST(BlockTypeTest, AllBlocksHaveDistinctVisuals) {
    auto stone = Block::get(BLOCK_STONE);
    auto dirt = Block::get(BLOCK_DIRT);
    auto grass = Block::get(BLOCK_GRASS);
    auto sand = Block::get(BLOCK_SAND);
    auto wood = Block::get(BLOCK_WOOD);
    auto leaves = Block::get(BLOCK_LEAVES);
    auto bedrock = Block::get(BLOCK_BEDROCK);

    EXPECT_NE(stone.sideColor, dirt.sideColor);
    EXPECT_NE(dirt.sideColor, sand.sideColor);
    EXPECT_NE(stone.sideColor, sand.sideColor);
    EXPECT_NE(wood.sideColor, leaves.sideColor);
    EXPECT_NE(stone.topColor, grass.topColor);
    EXPECT_NE(bedrock.topColor, stone.topColor);
}

TEST(BlockTypeTest, GrassTopDifferentFromSideAndBottom) {
    auto grass = Block::get(BLOCK_GRASS);
    EXPECT_NE(grass.topColor, grass.sideColor);
    EXPECT_NE(grass.topColor, grass.bottomColor);
    EXPECT_NE(grass.sideColor, grass.bottomColor);
    EXPECT_FLOAT_EQ(grass.sideColor.r, 0.38f);
    EXPECT_FLOAT_EQ(grass.topColor.g, 0.62f);
}

TEST(BlockTypeTest, DirtIsUniformColor) {
    auto dirt = Block::get(BLOCK_DIRT);
    EXPECT_EQ(dirt.topColor, dirt.sideColor);
    EXPECT_EQ(dirt.sideColor, dirt.bottomColor);
}

TEST(BlockTypeTest, StoneIsGrey) {
    auto stone = Block::get(BLOCK_STONE);
    EXPECT_FLOAT_EQ(stone.topColor.r, 0.50f);
    EXPECT_FLOAT_EQ(stone.topColor.g, 0.50f);
    EXPECT_FLOAT_EQ(stone.topColor.b, 0.52f);
    EXPECT_EQ(stone.topColor, stone.sideColor);
}

TEST(BlockTypeTest, WaterIsTransparentNotSolid) {
    auto water = Block::get(BLOCK_WATER);
    EXPECT_TRUE(water.transparent);
    EXPECT_FALSE(water.solid);
}

TEST(BlockTypeTest, LeavesAreTransparent) {
    auto leaves = Block::get(BLOCK_LEAVES);
    EXPECT_TRUE(leaves.transparent);
    EXPECT_TRUE(leaves.solid);
}

TEST(BlockTypeTest, BedrockUnbreakable) {
    auto bedrock = Block::get(BLOCK_BEDROCK);
    EXPECT_LT(bedrock.hardness, 0.0f);
}

TEST(BlockTypeTest, ChunkMeshEncodesBlockId) {
    Chunk chunk(0, 0);
    chunk.setBlock(0, 0, 0, BLOCK_STONE);
    chunk.setBlock(2, 0, 0, BLOCK_DIRT);
    chunk.generateMesh();
    ASSERT_FALSE(chunk.isEmpty());
    bool foundStone = false;
    bool foundDirt = false;
    for (size_t i = 0; i + 4 < chunk.getMeshForTest().vertices.size(); i += 5) {
        float blockId = chunk.getMeshForTest().vertices[i + 4];
        if (blockId == static_cast<float>(BLOCK_STONE)) foundStone = true;
        if (blockId == static_cast<float>(BLOCK_DIRT)) foundDirt = true;
    }
    EXPECT_TRUE(foundStone);
    EXPECT_TRUE(foundDirt);
}

TEST(BlockTypeTest, ChunkMeshHasFiveFloatsPerVertex) {
    Chunk chunk(0, 0);
    chunk.setBlock(0, 0, 0, BLOCK_GRASS);
    chunk.generateMesh();
    auto &mesh = chunk.getMeshForTest();
    EXPECT_EQ(mesh.vertices.size() % 5, 0u);
    EXPECT_GT(mesh.vertices.size(), 0u);
}
