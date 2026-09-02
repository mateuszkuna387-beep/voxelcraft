#include <gtest/gtest.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "engine/Engine.h"
#include "engine/Input.h"
#include "engine/Window.h"
#include "player/Player.h"
#include "world/World.h"
#include "gui/Menu.h"
#include "core/Constants.h"

class CameraResumeTest : public ::testing::Test {
protected:
    static GLFWwindow* s_window;
    static Engine* s_engine;

    static void SetUpTestSuite() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        s_window = glfwCreateWindow(800, 600, "CameraResumeTest", nullptr, nullptr);
        ASSERT_NE(s_window, nullptr);
        glfwMakeContextCurrent(s_window);
        ASSERT_TRUE(gladLoadGL(glfwGetProcAddress));

        s_engine = new Engine();
        ASSERT_TRUE(s_engine->init(800, 600, "CameraResumeTest"));
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
    }

    void simulateMouseMove(f64 dx, f64 dy) {
        s_engine->input().setMouseDeltaForTesting(dx, dy);
    }

    void pressAndReleaseEscape() {
        s_engine->window().setCursorMode(GLFW_CURSOR_NORMAL);
        glfwPollEvents();
        s_engine->input().resetMouseDelta();
        s_engine->window().setCursorMode(GLFW_CURSOR_DISABLED);
        glfwPollEvents();
    }

    void runFrame() {
        s_engine->handleInput(0.016f);
        s_engine->update(0.016f);
    }
};

GLFWwindow* CameraResumeTest::s_window = nullptr;
Engine* CameraResumeTest::s_engine = nullptr;

TEST_F(CameraResumeTest, CameraRotationPreservedAfterPauseResume) {
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    s_engine->player()->setRotation(glm::vec3(0.5f, 1.2f, 0.0f));
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    f32 initialPitch = s_engine->camera()->rotation().x;
    f32 initialYaw = s_engine->camera()->rotation().y;

    simulateMouseMove(100, 50);
    runFrame();

    f32 afterMovePitch = s_engine->camera()->rotation().x;
    f32 afterMoveYaw = s_engine->camera()->rotation().y;

    EXPECT_NE(afterMovePitch, initialPitch);
    EXPECT_NE(afterMoveYaw, initialYaw);

    pressAndReleaseEscape();

    f32 afterResumePitch = s_engine->camera()->rotation().x;
    f32 afterResumeYaw = s_engine->camera()->rotation().y;

    EXPECT_FLOAT_EQ(afterResumePitch, afterMovePitch) << "Pitch should not change after resume";
    EXPECT_FLOAT_EQ(afterResumeYaw, afterMoveYaw) << "Yaw should not change after resume";
}

TEST_F(CameraResumeTest, CameraRotationPreservedAfterPauseResumeWithMovement) {
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    s_engine->player()->setRotation(glm::vec3(0.3f, 2.0f, 0.0f));
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    simulateMouseMove(200, -100);
    runFrame();

    f32 pitchBeforePause = s_engine->camera()->rotation().x;
    f32 yawBeforePause = s_engine->camera()->rotation().y;

    pressAndReleaseEscape();

    f32 pitchAfterResume = s_engine->camera()->rotation().x;
    f32 yawAfterResume = s_engine->camera()->rotation().y;

    EXPECT_FLOAT_EQ(pitchAfterResume, pitchBeforePause) << "Pitch should not change after resume";
    EXPECT_FLOAT_EQ(yawAfterResume, yawBeforePause) << "Yaw should not change after resume";
}

TEST_F(CameraResumeTest, NoCameraJumpWhenOpeningAndClosingMenu) {
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    s_engine->player()->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    f32 initialPitch = s_engine->camera()->rotation().x;
    f32 initialYaw = s_engine->camera()->rotation().y;

    for (int i = 0; i < 5; ++i) {
        pressAndReleaseEscape();
    }

    f32 finalPitch = s_engine->camera()->rotation().x;
    f32 finalYaw = s_engine->camera()->rotation().y;

    EXPECT_FLOAT_EQ(finalPitch, initialPitch) << "Pitch should not drift after multiple pause/resume cycles";
    EXPECT_FLOAT_EQ(finalYaw, initialYaw) << "Yaw should not drift after multiple pause/resume cycles";
}

TEST_F(CameraResumeTest, CameraPositionPreservedAfterPauseResume) {
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    s_engine->player()->setPosition(glm::vec3(100.0f, 70.0f, 200.0f));
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    glm::vec3 initialPos = s_engine->camera()->position();

    pressAndReleaseEscape();

    glm::vec3 afterResumePos = s_engine->camera()->position();

    EXPECT_FLOAT_EQ(afterResumePos.x, initialPos.x) << "Camera X position should not change after resume";
    EXPECT_FLOAT_EQ(afterResumePos.y, initialPos.y) << "Camera Y position should not change after resume";
    EXPECT_FLOAT_EQ(afterResumePos.z, initialPos.z) << "Camera Z position should not change after resume";
}

TEST_F(CameraResumeTest, CameraViewMatrixPreservedAfterPauseResume) {
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    s_engine->player()->setPosition(glm::vec3(50.0f, 65.0f, 150.0f));
    s_engine->player()->setRotation(glm::vec3(0.2f, 1.5f, 0.0f));
    s_engine->camera()->follow(s_engine->player()->position(), s_engine->player()->rotation());

    glm::mat4 viewBeforePause = s_engine->camera()->viewMatrix();

    pressAndReleaseEscape();

    glm::mat4 viewAfterResume = s_engine->camera()->viewMatrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(viewAfterResume[i][j], viewBeforePause[i][j])
                << "View matrix element [" << i << "][" << j << "] should match after resume";
        }
    }
}