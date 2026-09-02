#include <gtest/gtest.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "engine/Engine.h"
#include "world/World.h"
#include "core/Constants.h"

class CenterSelectionTest : public ::testing::Test {
protected:
    static GLFWwindow* s_window;
    static Engine* s_engine;

    static void SetUpTestSuite() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        s_window = glfwCreateWindow(800, 600, "CenterSelectionTest", nullptr, nullptr);
        ASSERT_NE(s_window, nullptr);
        glfwMakeContextCurrent(s_window);
        ASSERT_TRUE(gladLoadGL(glfwGetProcAddress));
        s_engine = new Engine();
        ASSERT_TRUE(s_engine->init(800, 600, "CenterSelectionTest"));
    }
    static void TearDownTestSuite() {
        if (s_engine) { s_engine->shutdown(); delete s_engine; }
        glfwDestroyWindow(s_window);
        glfwTerminate();
    }
    void SetUp() override {
        for (int i=0;i<5;i++){ s_engine->handleInput(0.016f); s_engine->update(0.016f); }
    }
    void runFrames(int n=1){ for(int i=0;i<n;i++){ s_engine->handleInput(0.016f); s_engine->update(0.016f);} }
    void setPlayer(glm::vec3 pos, glm::vec3 rot){
        s_engine->input().resetMouseDelta();
        s_engine->player()->setPosition(pos);
        s_engine->player()->setRotation(rot);
        runFrames(2);
    }
    glm::vec3 dirFromRot(glm::vec3 rot){
        glm::vec3 d; d.x = std::sin(rot.y)*std::cos(rot.x); d.y = -std::sin(rot.x); d.z = -std::cos(rot.y)*std::cos(rot.x);
        return glm::normalize(d);
    }
};

GLFWwindow* CenterSelectionTest::s_window = nullptr;
Engine* CenterSelectionTest::s_engine = nullptr;

TEST_F(CenterSelectionTest, RayHitsCenterNotLeftAtIntegerOrigin) {
    World* w = s_engine->world();
    for(int x=0;x<32;x++) for(int y=0;y<WORLD_SIZE_Y;y++) for(int z=0;z<32;z++) w->setBlock(x,y,z,BLOCK_AIR);
    for(int cx=0;cx<2;cx++) for(int cz=0;cz<2;cz++) if(!w->getChunk(cx,cz)) w->loadChunk(cx,cz);
    w->setBlock(5,2,0,BLOCK_STONE);
    w->setBlock(4,2,0,BLOCK_STONE);
    w->setBlock(6,2,0,BLOCK_STONE);
    setPlayer(glm::vec3(5,1,5), glm::vec3(0,0,0));
    EXPECT_TRUE(s_engine->hasBlockTarget());
    auto hit = s_engine->targetBlock();
    EXPECT_EQ(hit.x, 5) << "Integer origin at x=5 looking -Z should hit center block (5,2,0) not left neighbor";
    EXPECT_EQ(hit.z, 0);
}

TEST_F(CenterSelectionTest, RayHitsCenterAtHalfIntegerOrigin) {
    World* w = s_engine->world();
    for(int x=0;x<32;x++) for(int y=0;y<WORLD_SIZE_Y;y++) for(int z=0;z<32;z++) w->setBlock(x,y,z,BLOCK_AIR);
    w->setBlock(5,2,0,BLOCK_STONE);
    w->setBlock(4,2,0,BLOCK_STONE);
    setPlayer(glm::vec3(5.5f,1,5.5f), glm::vec3(0,0,0));
    EXPECT_TRUE(s_engine->hasBlockTarget());
    auto hit = s_engine->targetBlock();
    EXPECT_EQ(hit.x, 5);
}

TEST_F(CenterSelectionTest, SmallYawDoesNotCauseLeftBias) {
    World w;
    for(int cx=0;cx<2;cx++) for(int cz=0;cz<2;cz++) w.loadChunk(cx,cz);
    for(int x=0;x<32;x++) for(int y=0;y<WORLD_SIZE_Y;y++) for(int z=0;z<32;z++) w.setBlock(x,y,z,BLOCK_AIR);
    w.setBlock(5,2,0,BLOCK_STONE);
    w.setBlock(4,2,0,BLOCK_STONE);
    w.setBlock(6,2,0,BLOCK_STONE);
    glm::vec3 origin(5.0f, 2.6f, 5.0f);
    auto dirForYaw = [](float yaw){ glm::vec3 d; d.x = std::sin(yaw); d.y = 0; d.z = -std::cos(yaw); return glm::normalize(d); };
    int hx,hy,hz,px,py,pz;
    bool has = w.raycast(origin, dirForYaw(0.05f), 10, hx,hy,hz, px,py,pz);
    EXPECT_TRUE(has) << "Yaw 0.05 should hit wall within 10";
    if(has) { EXPECT_GE(hx, 4); EXPECT_LE(hx, 6); }
    has = w.raycast(origin, dirForYaw(-0.05f), 10, hx,hy,hz, px,py,pz);
    EXPECT_TRUE(has) << "Yaw -0.05 should hit wall within 10";
    if(has) { EXPECT_GE(hx, 4); EXPECT_LE(hx, 6); }
    has = w.raycast(origin, dirForYaw(0.0f), 10, hx,hy,hz, px,py,pz);
    EXPECT_TRUE(has);
    EXPECT_EQ(hx, 5);
}

TEST_F(CenterSelectionTest, ViewForwardMatchesRayDir) {
    std::vector<glm::vec3> rots = {
        {0,0,0}, {0.2f,0,0}, {-0.2f,0,0}, {0,0.5f,0}, {0,-0.5f,0}, {0.2f,0.5f,0}, {0.2f,-0.5f,0}, {0.5f,1.0f,0}
    };
    for(auto rot: rots){
        setPlayer(glm::vec3(10,5,10), rot);
        glm::vec3 dir = dirFromRot(s_engine->camera()->rotation());
        glm::mat4 view = s_engine->camera()->viewMatrix();
        glm::mat4 inv = glm::inverse(view);
        glm::vec3 fwd = glm::normalize(glm::vec3(inv * glm::vec4(0,0,-1,0)));
        float dot = glm::dot(dir, fwd);
        EXPECT_NEAR(dot, 1.0f, 0.001f) << "rot pitch="<<rot.x<<" yaw="<<rot.y<<" dir vs view forward mismatch would cause left/right offset";
    }
}

TEST_F(CenterSelectionTest, DirectWorldRaycastIntegerBoundary) {
    World w;
    for(int cx=0;cx<2;cx++) for(int cz=0;cz<2;cz++) w.loadChunk(cx,cz);
    for(int x=0;x<32;x++) for(int y=0;y<WORLD_SIZE_Y;y++) for(int z=0;z<32;z++) w.setBlock(x,y,z,BLOCK_AIR);
    w.setBlock(5,5,5,BLOCK_STONE);
    w.setBlock(4,5,5,BLOCK_STONE);
    glm::vec3 origin(5.0f,5.5f,10.0f);
    glm::vec3 dir(0,0,-1);
    int hx,hy,hz,px,py,pz;
    bool has = w.raycast(origin, dir, 10, hx,hy,hz, px,py,pz);
    ASSERT_TRUE(has);
    EXPECT_EQ(hx, 5) << "Origin exactly on integer X boundary looking -Z should hit block at X=5 not left neighbor 4";
    EXPECT_EQ(hz, 5);
}

TEST_F(CenterSelectionTest, HighlightUsesEyePosition) {
    World* w = s_engine->world();
    for(int x=0;x<32;x++) for(int y=0;y<WORLD_SIZE_Y;y++) for(int z=0;z<32;z++) w->setBlock(x,y,z,BLOCK_AIR);
    w->setBlock(5,2,3,BLOCK_STONE);
    setPlayer(glm::vec3(5,1,5), glm::vec3(0,0,0));
    ASSERT_TRUE(s_engine->hasBlockTarget());
    auto hit = s_engine->targetBlock();
    EXPECT_EQ(hit.x, 5);
    EXPECT_EQ(hit.z, 3);
    glm::vec3 camPos = s_engine->camera()->position();
    glm::vec3 targetPos = glm::vec3(hit) + glm::vec3(0.5f);
    float distEye = glm::length(camPos - targetPos);
    float distFeet = glm::length(s_engine->player()->position() - targetPos);
    EXPECT_LE(distEye, BLOCK_HIGHLIGHT_DISTANCE) << "Eye distance should be within highlight range for close block";
    EXPECT_NE(distEye, distFeet);
}
