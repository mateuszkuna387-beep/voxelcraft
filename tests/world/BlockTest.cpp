#include <gtest/gtest.h>

#include "world/Block.h"
#include "core/Constants.h"

TEST(BlockTest, AirIsTransparent) {
    const BlockData& air = Block::get(BLOCK_AIR);
    EXPECT_TRUE(air.transparent);
    EXPECT_FALSE(air.solid);
}

TEST(BlockTest, StoneIsSolid) {
    const BlockData& stone = Block::get(BLOCK_STONE);
    EXPECT_FALSE(stone.transparent);
    EXPECT_TRUE(stone.solid);
    EXPECT_FLOAT_EQ(stone.hardness, 2.0f);
}

TEST(BlockTest, UnknownBlockReturnsAir) {
    const BlockData& unknown = Block::get(999);
    EXPECT_TRUE(unknown.transparent);
    EXPECT_FALSE(unknown.solid);
}
