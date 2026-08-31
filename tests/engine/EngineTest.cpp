#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Constants.h"

TEST(EngineTest, BlockHighlightDistanceWithinRange) {
    glm::vec3 playerPos(5.0f, 1.0f, 5.0f);
    glm::vec3 targetPos(5.5f, 1.5f, 5.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_LE(dist, BLOCK_HIGHLIGHT_DISTANCE);
}

TEST(EngineTest, BlockHighlightDistanceAtBoundary) {
    glm::vec3 playerPos(0.0f, 0.0f, 0.0f);
    glm::vec3 targetPos(2.5f, 0.5f, 0.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_LE(dist, BLOCK_HIGHLIGHT_DISTANCE + 0.01f);
}

TEST(EngineTest, BlockHighlightDistanceOutsideRange) {
    glm::vec3 playerPos(0.0f, 0.0f, 0.0f);
    glm::vec3 targetPos(4.0f + 0.5f, 0.5f, 0.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_GT(dist, BLOCK_HIGHLIGHT_DISTANCE);
}

TEST(EngineTest, BlockHighlightDistanceDiagonal) {
    glm::vec3 playerPos(5.0f, 1.0f, 5.0f);
    glm::vec3 targetPos(7.5f, 2.5f, 7.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_GT(dist, BLOCK_HIGHLIGHT_DISTANCE);
}

TEST(EngineTest, BlockHighlightDistanceVertical) {
    glm::vec3 playerPos(5.0f, 1.0f, 5.0f);
    glm::vec3 targetPos(5.5f, 4.5f, 5.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_GT(dist, BLOCK_HIGHLIGHT_DISTANCE);
}

TEST(EngineTest, BlockHighlightDistanceClose) {
    glm::vec3 playerPos(5.0f, 1.0f, 5.0f);
    glm::vec3 targetPos(5.5f, 1.5f, 6.5f);

    f32 dist = glm::length(playerPos - targetPos);

    EXPECT_LE(dist, BLOCK_HIGHLIGHT_DISTANCE);
}