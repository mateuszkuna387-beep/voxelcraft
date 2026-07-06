#include <gtest/gtest.h>

#include "rendering/Camera.h"

TEST(CameraTest, DefaultPosition) {
    Camera cam(800, 600);
    EXPECT_FLOAT_EQ(cam.position().x, 0.0f);
    EXPECT_FLOAT_EQ(cam.position().y, 0.0f);
    EXPECT_FLOAT_EQ(cam.position().z, 0.0f);
}

TEST(CameraTest, SetPosition) {
    Camera cam(800, 600);
    cam.setPosition(glm::vec3(10.0f, 20.0f, 30.0f));
    EXPECT_FLOAT_EQ(cam.position().x, 10.0f);
    EXPECT_FLOAT_EQ(cam.position().y, 20.0f);
    EXPECT_FLOAT_EQ(cam.position().z, 30.0f);
}
