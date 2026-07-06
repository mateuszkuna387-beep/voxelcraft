#include <gtest/gtest.h>

#include "player/Player.h"

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
