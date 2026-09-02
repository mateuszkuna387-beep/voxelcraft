#include <gtest/gtest.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

#include "engine/Engine.h"
#include "engine/Input.h"
#include "engine/Window.h"
#include "player/Player.h"
#include "world/World.h"
#include "core/Constants.h"

class RaycastTest : public ::testing::Test {
protected:
    static GLFWwindow* s_window;
    static Engine* s_engine;

    static void SetUpTestSuite() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        s_window = glfwCreateWindow(800, 600, "RaycastTest", nullptr, nullptr);
        ASSERT_NE(s_window, nullptr);
        glfwMakeContextCurrent(s_window);
        ASSERT_TRUE(gladLoadGL(glfwGetProcAddress));

        s_engine = new Engine();
        ASSERT_TRUE(s_engine->init(800, 600, "RaycastTest"));
    }

    static void TearDownTestSuite() {
        if (s_engine) {
            s_engine->shutdown();
            delete s_engine;
        }
        glfwDestroyWindow(s_window);
        glfwTerminate();
    }

    void SetUp() override {
        glfwSetCursorPos(s_window, 400, 300);
        glfwPollEvents();
        
        // Run a few frames to let everything initialize
        for (int i = 0; i < 10; ++i) {
            s_engine->handleInput(0.016f);
            s_engine->update(0.016f);
        }
    }

    void runFrames(int count = 1) {
        for (int i = 0; i < count; ++i) {
            s_engine->handleInput(0.016f);
            s_engine->update(0.016f);
        }
    }

    void setPlayerRotation(const glm::vec3& rot) {
        s_engine->player()->setRotation(rot);
        runFrames(1);
    }

    void setPlayerPosition(const glm::vec3& pos) {
        s_engine->player()->setPosition(pos);
        runFrames(1);
    }

    glm::vec3 getCameraDirection() const {
        glm::vec3 rot = s_engine->camera()->rotation();
        glm::vec3 dir;
        dir.x = std::sin(rot.y) * std::cos(rot.x);
        dir.y = -std::sin(rot.x);
        dir.z = -std::cos(rot.y) * std::cos(rot.x);
        return glm::normalize(dir);
    }

    bool hasTarget() const {
        return s_engine->hasBlockTarget();
    }

    glm::ivec3 getTargetBlock() const {
        return s_engine->targetBlock();
    }

    glm::ivec3 getPlaceBlock() const {
        return s_engine->placeBlock();
    }

    void simulateMouseMove(f64 dx, f64 dy) {
        s_engine->input().setMouseDeltaForTesting(dx, dy);
        runFrames(1);
    }
};

GLFWwindow* RaycastTest::s_window = nullptr;
Engine* RaycastTest::s_engine = nullptr;

TEST_F(RaycastTest, RaycastHitsBlockDirectlyInFront) {
    // Position player on top of terrain at x=10, z=10
    // Terrain height at (10, 10) is ~35-65, so use y=60 to be within raycast distance
    setPlayerPosition(glm::vec3(10.0f, 60.0f, 10.0f));
    // Look straight down (pitch = pi/2) — negative pitch is up after fix to match view matrix
    setPlayerRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));

    runFrames(10);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    // Should hit a block at roughly the surface level
    EXPECT_GT(target.y, 30);
    EXPECT_LT(target.y, 70);
}

TEST_F(RaycastTest, RaycastHitsBlockLookingPositiveX) {
    // Position player at height where horizontal ray hits terrain within 10 blocks
    // Terrain at x=10,z=10 is around y=54, so position at y=55 and look horizontally
    setPlayerPosition(glm::vec3(10.0f, 55.0f, 10.0f));
    // Look along +X (rot.y = -pi/2), slightly down
    setPlayerRotation(glm::vec3(0.3f, -glm::half_pi<float>(), 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    EXPECT_GT(target.y, 30);
    EXPECT_LT(target.y, 70);
}

TEST_F(RaycastTest, RaycastHitsBlockLookingPositiveZ) {
    // Position closer to terrain at z=10, look along +Z with steeper downward angle
    setPlayerPosition(glm::vec3(10.0f, 60.0f, 10.0f));
    // Look along +Z (rot.y = pi), steeper down to hit terrain within 10 blocks
    setPlayerRotation(glm::vec3(0.8f, glm::pi<float>(), 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    EXPECT_GT(target.y, 30);
    EXPECT_LT(target.y, 70);
}

TEST_F(RaycastTest, RaycastHitsBlockLookingNegativeZ) {
    setPlayerPosition(glm::vec3(10.0f, 55.0f, 10.0f));
    // Look along -Z (rot.y = 0), slightly down
    setPlayerRotation(glm::vec3(0.3f, 0.0f, 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    EXPECT_GT(target.y, 30);
    EXPECT_LT(target.y, 70);
}

TEST_F(RaycastTest, RaycastDirectionMatchesPlayerRotation) {
    setPlayerPosition(glm::vec3(10.0f, 5.0f, 10.0f));

    glm::vec3 testRotations[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, glm::half_pi<float>(), 0.0f},
        {0.0f, -glm::half_pi<float>(), 0.0f},
        {0.0f, glm::pi<float>(), 0.0f},
        {glm::quarter_pi<float>(), 0.0f, 0.0f},
        {-glm::quarter_pi<float>(), 0.0f, 0.0f},
    };

    for (const auto& rot : testRotations) {
        setPlayerRotation(rot);
        runFrames(2);

        glm::vec3 playerRot = s_engine->player()->rotation();
        glm::vec3 dir = getCameraDirection();

        float expectedDirX = std::sin(playerRot.y) * std::cos(playerRot.x);
        float expectedDirY = -std::sin(playerRot.x);
        float expectedDirZ = -std::cos(playerRot.y) * std::cos(playerRot.x);
        glm::vec3 expectedDir(expectedDirX, expectedDirY, expectedDirZ);
        expectedDir = glm::normalize(expectedDir);

        EXPECT_NEAR(dir.x, expectedDir.x, 0.001f) << "For rotation: " << rot.x << ", " << rot.y;
        EXPECT_NEAR(dir.y, expectedDir.y, 0.001f) << "For rotation: " << rot.x << ", " << rot.y;
        EXPECT_NEAR(dir.z, expectedDir.z, 0.001f) << "For rotation: " << rot.x << ", " << rot.y;
    }
}

TEST_F(RaycastTest, RaycastRespectsMaxDistance) {
    // Position at y=60, surface at ~54, distance = 6 which is < 10
    setPlayerPosition(glm::vec3(50.0f, 60.0f, 50.0f));
    // Look straight down
    setPlayerRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    float dist = glm::length(glm::vec3(target) + glm::vec3(0.5f) - s_engine->camera()->position());
    EXPECT_LE(dist, RAYCAST_DISTANCE + 1.0f);
}

TEST_F(RaycastTest, NoTargetWhenLookingAtAir) {
    setPlayerPosition(glm::vec3(100.0f, 200.0f, 100.0f));
    setPlayerRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    runFrames(5);

    EXPECT_FALSE(hasTarget());
}

TEST_F(RaycastTest, PlaceBlockPositionIsAdjacentToTarget) {
    // Look straight down to hit top face cleanly
    setPlayerPosition(glm::vec3(10.0f, 60.0f, 10.0f));
    setPlayerRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());
    glm::ivec3 target = getTargetBlock();
    glm::ivec3 place = getPlaceBlock();

    int dx = std::abs(target.x - place.x);
    int dy = std::abs(target.y - place.y);
    int dz = std::abs(target.z - place.z);

    int adj = dx + dy + dz;
    EXPECT_EQ(adj, 1);
}

TEST_F(RaycastTest, CrosshairPointsToTargetBlock) {
    setPlayerPosition(glm::vec3(10.0f, 5.0f, 10.0f));
    setPlayerRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    runFrames(5);

    EXPECT_TRUE(hasTarget());

    glm::vec3 camPos = s_engine->camera()->position();
    glm::vec3 targetPos = glm::vec3(getTargetBlock()) + glm::vec3(0.5f);
    glm::vec3 toTarget = glm::normalize(targetPos - camPos);
    glm::vec3 camDir = getCameraDirection();

    float dot = glm::dot(toTarget, camDir);
    EXPECT_GT(dot, 0.99f);
}