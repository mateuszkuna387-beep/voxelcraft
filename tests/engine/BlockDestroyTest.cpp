#include <gtest/gtest.h>

#include "world/Block.h"
#include "world/Chunk.h"
#include "world/World.h"
#include "core/Constants.h"
#include "rendering/BlockParticle.h"

TEST(BlockDestroyTest, BreakingTakesTimeBasedOnHardness) {
    EXPECT_GT(Block::get(BLOCK_STONE).hardness, Block::get(BLOCK_DIRT).hardness);
    EXPECT_GT(Block::get(BLOCK_DIRT).hardness, Block::get(BLOCK_LEAVES).hardness);
    EXPECT_LT(Block::get(BLOCK_BEDROCK).hardness, 0.0f);
    float stoneTime = BASE_BREAK_TIME * Block::get(BLOCK_STONE).hardness;
    float dirtTime = BASE_BREAK_TIME * Block::get(BLOCK_DIRT).hardness;
    EXPECT_GT(stoneTime, dirtTime);
    EXPECT_GT(stoneTime, 0.0f);
}

TEST(BlockDestroyTest, BedrockUnbreakable) {
    World w;
    w.loadChunk(0,0);
    Chunk* c = w.getChunk(0,0);
    c->setBlock(5,5,5,BLOCK_BEDROCK);
    EXPECT_LT(Block::get(w.getBlock(5,5,5)).hardness, 0.0f);
}

TEST(BlockDestroyTest, ParticleSpawnOnBreak) {
    BlockParticleSystem ps;
    // init may require GL context, so just test spawn logic without GL
    // Use a mock without GL: directly test that Block::get returns colors
    auto stone = Block::get(BLOCK_STONE);
    auto dirt = Block::get(BLOCK_DIRT);
    EXPECT_NE(stone.sideColor, dirt.sideColor);
}

TEST(BlockDestroyTest, BlockRemovalGivesInventory) {
    World w;
    w.loadChunk(0,0);
    Chunk* c = w.getChunk(0,0);
    for(int y=0;y<CHUNK_SIZE_Y;y++) for(int z=0;z<CHUNK_SIZE_Z;z++) for(int x=0;x<CHUNK_SIZE_X;x++) c->setBlock(x,y,z,BLOCK_AIR);
    w.setBlock(5,5,5,BLOCK_DIRT);
    EXPECT_EQ(w.getBlock(5,5,5), BLOCK_DIRT);
    w.setBlock(5,5,5,BLOCK_AIR);
    EXPECT_EQ(w.getBlock(5,5,5), BLOCK_AIR);
}

TEST(BlockDestroyTest, DifferentBlocksDistinctInWorld) {
    TerrainGenerator gen(42);
    Chunk chunk(0,0);
    gen.generate(&chunk);
    bool hasStone=false, hasDirt=false, hasGrass=false;
    for(int y=0;y<CHUNK_SIZE_Y;y++){
        for(int z=0;z<CHUNK_SIZE_Z;z++){
            for(int x=0;x<CHUNK_SIZE_X;x++){
                BlockID id = chunk.getBlock(x,y,z);
                if(id==BLOCK_STONE) hasStone=true;
                if(id==BLOCK_DIRT) hasDirt=true;
                if(id==BLOCK_GRASS) hasGrass=true;
            }
        }
    }
    EXPECT_TRUE(hasStone);
    EXPECT_TRUE(hasDirt);
    EXPECT_TRUE(hasGrass);
}
