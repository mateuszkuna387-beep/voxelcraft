#include <gtest/gtest.h>

#include "player/Player.h"
#include "world/World.h"
#include "world/Chunk.h"
#include "core/Constants.h"

static void clearChunk(Chunk* chunk) {
    for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                chunk->setBlock(x, y, z, BLOCK_AIR);
            }
        }
    }
}

static void placePlatform(World* world, i32 height, i32 size) {
    for (i32 z = 0; z < size; ++z) {
        for (i32 x = 0; x < size; ++x) {
            world->setBlock(x, height, z, BLOCK_STONE);
        }
    }
}

TEST(PlayerTest, DefaultPosition) {
    Player player;
    EXPECT_FLOAT_EQ(player.position().y, 64.0f);
}

TEST(PlayerTest, DefaultRotation) {
    Player player;
    EXPECT_FLOAT_EQ(player.rotation().x, 0.0f);
    EXPECT_FLOAT_EQ(player.rotation().y, 0.0f);
}

TEST(PlayerTest, SetPosition) {
    Player player;
    player.setPosition(glm::vec3(10.0f, 20.0f, 30.0f));
    EXPECT_FLOAT_EQ(player.position().x, 10.0f);
    EXPECT_FLOAT_EQ(player.position().y, 20.0f);
    EXPECT_FLOAT_EQ(player.position().z, 30.0f);
}

TEST(PlayerTest, SetRotation) {
    Player player;
    player.setRotation(glm::vec3(0.5f, 1.0f, 0.0f));
    EXPECT_FLOAT_EQ(player.rotation().x, 0.5f);
    EXPECT_FLOAT_EQ(player.rotation().y, 1.0f);
}

TEST(PlayerTest, DefaultNotOnGround) {
    Player player;
    EXPECT_FALSE(player.isOnGround());
}

TEST(PlayerTest, SetPositionAndRotation) {
    Player player;
    player.setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    player.setRotation(glm::vec3(0.1f, 0.2f, 0.0f));
    EXPECT_FLOAT_EQ(player.position().x, 1.0f);
    EXPECT_FLOAT_EQ(player.position().y, 2.0f);
    EXPECT_FLOAT_EQ(player.position().z, 3.0f);
    EXPECT_FLOAT_EQ(player.rotation().x, 0.1f);
    EXPECT_FLOAT_EQ(player.rotation().y, 0.2f);
}

TEST(PlayerTest, GravityPullsDown) {
    World world;
    Player player;

    player.setPosition(glm::vec3(5.0f, 20.0f, 5.0f));
    f32 initialY = player.position().y;

    player.update(0.05f, &world);

    EXPECT_LT(player.position().y, initialY);
    EXPECT_FALSE(player.isOnGround());
}

TEST(PlayerTest, StaysOnPlatformWhenStill) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    player.update(0.05f, &world);

    EXPECT_FLOAT_EQ(player.position().y, 1.0f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, LandsOnPlatform) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 2.5f, 5.0f));

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_NEAR(player.position().y, 1.0f, 0.01f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, JumpRisesThenFalls) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());
    ASSERT_FLOAT_EQ(player.position().y, 1.0f);

    player.move(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FALSE(player.isOnGround());

    f32 prevY = player.position().y;
    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GT(player.position().y, prevY);

    for (i32 i = 0; i < 50; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_FLOAT_EQ(player.position().y, 1.0f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, WalkForwardRelativeToRotation) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());
    ASSERT_FLOAT_EQ(player.position().y, 1.0f);

    f32 startX = player.position().x;
    f32 startZ = player.position().z;

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_LT(player.position().z, startZ);
    EXPECT_FLOAT_EQ(player.position().x, startX);
}

TEST(PlayerTest, StrafeRightRelativeToRotation) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());
    ASSERT_FLOAT_EQ(player.position().y, 1.0f);

    f32 startX = player.position().x;
    f32 startZ = player.position().z;

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GT(player.position().x, startX);
    EXPECT_FLOAT_EQ(player.position().z, startZ);
}

TEST(PlayerTest, MoveBackwardRelativeToRotation) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());
    ASSERT_FLOAT_EQ(player.position().y, 1.0f);

    f32 startX = player.position().x;
    f32 startZ = player.position().z;

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(0.0f, 0.0f, -1.0f), 0.0f);

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GT(player.position().z, startZ);
    EXPECT_FLOAT_EQ(player.position().x, startX);
}

TEST(PlayerTest, WalkForwardAfterRotation) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());

    player.setRotation(glm::vec3(0.0f, 1.5707963f, 0.0f));
    player.move(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);

    f32 startX = player.position().x;
    f32 startZ = player.position().z;

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GT(player.position().x, startX);
    EXPECT_FLOAT_EQ(player.position().z, startZ);
}

TEST(PlayerTest, WallBlocksRightStrafe) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    world.setBlock(7, 1, 5, BLOCK_STONE);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);

    for (i32 i = 0; i < 30; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_LE(player.position().x, 7.0f - PLAYER_RADIUS + 0.01f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, WallBlocksLeftStrafe) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    world.setBlock(3, 1, 5, BLOCK_STONE);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(-1.0f, 0.0f, 0.0f), 0.0f);

    for (i32 i = 0; i < 30; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GE(player.position().x, 3.0f + PLAYER_RADIUS - 0.01f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, DoesNotClipThroughWall) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    world.setBlock(7, 1, 5, BLOCK_STONE);

    Player player;
    player.setPosition(glm::vec3(6.5f, 1.0f, 5.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());
    ASSERT_FLOAT_EQ(player.position().y, 1.0f);

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);

    for (i32 i = 0; i < 20; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_LE(player.position().x, 7.0f - PLAYER_RADIUS + 0.01f);
    EXPECT_NEAR(player.position().y, 1.0f, 0.05f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, MovePreservesYAbovePlatform) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(3.0f, 1.0f, 3.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    player.move(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);

    for (i32 i = 0; i < 10; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_TRUE(player.isOnGround());
    EXPECT_FLOAT_EQ(player.position().y, 1.0f);
}

TEST(PlayerTest, DoesNotSinkOverTime) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 500; ++i) {
        player.update(0.05f, &world);
        EXPECT_GE(player.position().y, 0.99f);
    }

    EXPECT_NEAR(player.position().y, 1.0f, 0.02f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, DoesNotSinkOnSingleBlock) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);

    world.setBlock(5, 0, 5, BLOCK_STONE);

    Player player;
    player.setPosition(glm::vec3(5.0f, 1.0f, 5.0f));

    for (i32 i = 0; i < 500; ++i) {
        player.update(0.05f, &world);
        EXPECT_GE(player.position().y, 0.99f);
    }

    EXPECT_NEAR(player.position().y, 1.0f, 0.02f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, DoesNotSinkWhileWalking) {
    World world;
    for (i32 cx = 0; cx < 4; ++cx) {
        for (i32 cz = 0; cz < 4; ++cz) {
            world.loadChunk(cx, cz);
            Chunk* c = world.getChunk(cx, cz);
            ASSERT_NE(c, nullptr);
            for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
                for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
                    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                        c->setBlock(x, y, z, BLOCK_AIR);
                    }
                }
            }
        }
    }
    placePlatform(&world, 0, 60);

    Player player;
    player.setPosition(glm::vec3(30.0f, 1.0f, 30.0f));

    for (i32 i = 0; i < 5; ++i) {
        player.update(0.05f, &world);
    }
    ASSERT_TRUE(player.isOnGround());

    player.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    for (i32 i = 0; i < 200; ++i) {
        if (i == 50) player.move(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
        if (i == 100) player.move(glm::vec3(0.0f, 0.0f, -1.0f), 0.0f);
        if (i == 150) player.move(glm::vec3(-1.0f, 0.0f, 0.0f), 0.0f);
        player.update(0.05f, &world);
        EXPECT_GE(player.position().y, 0.99f);
    }

    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, DoesNotSinkAfterLandingManyFrames) {
    World world;
    world.loadChunk(0, 0);
    Chunk* chunk = world.getChunk(0, 0);
    ASSERT_NE(chunk, nullptr);
    clearChunk(chunk);
    placePlatform(&world, 0, 10);

    Player player;
    player.setPosition(glm::vec3(5.0f, 10.0f, 5.0f));

    for (i32 i = 0; i < 500; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_NEAR(player.position().y, 1.0f, 0.02f);
    EXPECT_TRUE(player.isOnGround());
}

TEST(PlayerTest, DoesNotClipThroughGroundWhenFallingFromHeight) {
    World world;
    for (i32 cx = 0; cx < 4; ++cx) {
        for (i32 cz = 0; cz < 4; ++cz) {
            world.loadChunk(cx, cz);
            Chunk* c = world.getChunk(cx, cz);
            ASSERT_NE(c, nullptr);
            for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
                for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
                    for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                        c->setBlock(x, y, z, BLOCK_AIR);
                    }
                }
            }
        }
    }
    placePlatform(&world, 0, 60);

    Player player;
    player.setPosition(glm::vec3(30.0f, 99.0f, 30.0f));

    for (i32 i = 0; i < 1000; ++i) {
        player.update(0.05f, &world);
    }

    EXPECT_GE(player.position().y, 0.9f);
    EXPECT_TRUE(player.isOnGround());
}
