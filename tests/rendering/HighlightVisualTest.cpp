#include <gtest/gtest.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/Shader.h"
#include "core/Constants.h"

class HighlightVisualTest : public ::testing::Test {
protected:
    static GLFWwindow* s_window;
    static Shader* s_shader;
    static u32 s_vao, s_vbo, s_ebo;
    static int s_width, s_height;
    static bool s_headless;

    static void SetUpTestSuite() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, s_headless ? GLFW_FALSE : GLFW_TRUE);

        s_window = glfwCreateWindow(s_width, s_height, "HighlightVisualTest", nullptr, nullptr);
        ASSERT_NE(s_window, nullptr);
        glfwMakeContextCurrent(s_window);
        ASSERT_TRUE(gladLoadGL(glfwGetProcAddress));

        s_shader = new Shader();
        ASSERT_TRUE(s_shader->load("assets/shaders/block.vert", "assets/shaders/block.frag"));

        std::vector<f32> vertices = {
            0,0,0, 0,1,  1,0,0, 0,1,  1,1,0, 0,1,  0,1,0, 0,1,
            1,0,0, 1,1,  0,0,0, 1,1,  0,1,0, 1,1,  1,1,0, 1,1,
            1,0,0, 2,1,  1,0,1, 2,1,  1,1,1, 2,1,  1,1,0, 2,1,
            0,0,1, 3,1,  0,0,0, 3,1,  0,1,0, 3,1,  0,1,1, 3,1,
            0,1,1, 4,3,  1,1,1, 4,3,  1,1,0, 4,3,  0,1,0, 4,3,
            0,0,0, 5,1,  1,0,0, 5,1,  1,0,1, 5,1,  0,0,1, 5,1,
        };
        std::vector<u32> indices = {
            0,1,2, 0,2,3,  4,5,6, 4,6,7,  8,9,10, 8,10,11,
            12,13,14, 12,14,15,  16,17,18, 16,18,19,  20,21,22, 20,22,23
        };

        glGenVertexArrays(1, &s_vao);
        glGenBuffers(1, &s_vbo);
        glGenBuffers(1, &s_ebo);
        glBindVertexArray(s_vao);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(4 * sizeof(f32)));
        glEnableVertexAttribArray(2);
    }

    static void TearDownTestSuite() {
        if (s_vao) glDeleteVertexArrays(1, &s_vao);
        if (s_vbo) glDeleteBuffers(1, &s_vbo);
        if (s_ebo) glDeleteBuffers(1, &s_ebo);
        delete s_shader;
        glfwDestroyWindow(s_window);
        glfwTerminate();
    }

    void renderFrame(bool highlight, f32* outPixels) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        s_shader->use();
        s_shader->setFloat("uOverlay", 0.0f);
        s_shader->setFloat("uHighlight", highlight ? 1.0f : 0.0f);

        glm::mat4 proj = glm::perspective(glm::radians(75.0f), (f32)s_width / (f32)s_height, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0));
        glm::mat4 mvp = proj * view;
        s_shader->setMat4("uMVP", mvp);

        glBindVertexArray(s_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        if (outPixels) {
            glReadPixels(0, 0, s_width, s_height, GL_RGB, GL_FLOAT, outPixels);
        }
    }
};

GLFWwindow* HighlightVisualTest::s_window = nullptr;
Shader* HighlightVisualTest::s_shader = nullptr;
u32 HighlightVisualTest::s_vao = 0;
u32 HighlightVisualTest::s_vbo = 0;
u32 HighlightVisualTest::s_ebo = 0;
int HighlightVisualTest::s_width = 800;
int HighlightVisualTest::s_height = 600;
bool HighlightVisualTest::s_headless = true;

TEST_F(HighlightVisualTest, HighlightUniformChangesColor) {
    std::vector<f32> pixelsNoHighlight(s_width * s_height * 3);
    std::vector<f32> pixelsWithHighlight(s_width * s_height * 3);

    renderFrame(false, pixelsNoHighlight.data());
    renderFrame(true, pixelsWithHighlight.data());

    int diffCount = 0;
    for (size_t i = 0; i < pixelsNoHighlight.size(); ++i) {
        if (std::abs(pixelsNoHighlight[i] - pixelsWithHighlight[i]) > 0.01f) {
            diffCount++;
        }
    }

    EXPECT_GT(diffCount, 0) << "Highlight should change pixel colors";
}

TEST_F(HighlightVisualTest, HighlightBlendsWithWhite) {
    std::vector<f32> pixels(s_width * s_height * 3);
    renderFrame(true, pixels.data());

    int brightPixels = 0;
    for (size_t i = 0; i < pixels.size(); i += 3) {
        f32 r = pixels[i];
        f32 g = pixels[i + 1];
        f32 b = pixels[i + 2];
        if (r > 0.5f && g > 0.5f && b > 0.5f) {
            brightPixels++;
        }
    }

    EXPECT_GT(brightPixels, 0) << "Highlight should make pixels brighter (mixed with white)";
}

TEST_F(HighlightVisualTest, WireframeEdgesRendered) {
    std::vector<f32> pixels(s_width * s_height * 3);

    s_shader->use();
    s_shader->setFloat("uOverlay", 0.0f);
    s_shader->setFloat("uHighlight", 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 proj = glm::perspective(glm::radians(75.0f), (f32)s_width / (f32)s_height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0));
    glm::mat4 mvp = proj * view;
    s_shader->setMat4("uMVP", mvp);

    glBindVertexArray(s_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glReadPixels(0, 0, s_width, s_height, GL_RGB, GL_FLOAT, pixels.data());

    int edgePixels = 0;
    for (size_t i = 0; i < pixels.size(); i += 3) {
        f32 r = pixels[i];
        f32 g = pixels[i + 1];
        f32 b = pixels[i + 2];
        if (r > 0.02f || g > 0.02f || b > 0.02f) {
            edgePixels++;
        }
    }

    EXPECT_GT(edgePixels, 0) << "Wireframe edges should render non-black pixels";
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}