#include "Menu.h"
#include "Font.h"
#include "engine/Input.h"
#include "engine/Window.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Menu::Menu() = default;
Menu::~Menu() { destroy(); }

bool Menu::init() {
    m_shader = new Shader();
    if (!m_shader->load("assets/shaders/gui.vert", "assets/shaders/gui.frag")) {
        std::cerr << "Failed to load GUI shaders" << std::endl;
        return false;
    }

    m_font = new Font();
    if (!m_font->init()) {
        std::cerr << "Failed to initialize font" << std::endl;
        return false;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32),
                          reinterpret_cast<void*>(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    u8 whitePixel[4] = { 255, 255, 255, 255 };
    glGenTextures(1, &m_whiteTex);
    glBindTexture(GL_TEXTURE_2D, m_whiteTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_mousePressed = false;

    return true;
}

void Menu::destroy() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_whiteTex) glDeleteTextures(1, &m_whiteTex);
    m_vao = 0; m_vbo = 0; m_whiteTex = 0;

    delete m_font;
    m_font = nullptr;
    delete m_shader;
    m_shader = nullptr;
    m_buttons.clear();
}

void Menu::toggle() {
    m_open = !m_open;
}

void Menu::setupButtons(f32 screenWidth, f32 screenHeight) {
    m_buttons.clear();

    f32 btnW = 260.0f;
    f32 btnH = 50.0f;
    f32 centerX = screenWidth * 0.5f;
    f32 centerY = screenHeight * 0.5f;

    Button resumeBtn;
    resumeBtn.x = centerX - btnW * 0.5f;
    resumeBtn.y = centerY - btnH - 10.0f;
    resumeBtn.w = btnW;
    resumeBtn.h = btnH;
    resumeBtn.label = "wznuw gre";
    resumeBtn.callback = m_onResume;
    m_buttons.push_back(resumeBtn);

    Button exitBtn;
    exitBtn.x = centerX - btnW * 0.5f;
    exitBtn.y = centerY + 10.0f;
    exitBtn.w = btnW;
    exitBtn.h = btnH;
    exitBtn.label = "wyjdz z gry";
    exitBtn.callback = m_onExit;
    m_buttons.push_back(exitBtn);
}

void Menu::handleInput(Input& input, Window& window) {
    if (!m_open) return;

    f64 cursorX, cursorY;
    input.getMousePosition(cursorX, cursorY);

    for (auto& btn : m_buttons) {
        btn.hovered = (cursorX >= btn.x && cursorX <= btn.x + btn.w &&
                       cursorY >= btn.y && cursorY <= btn.y + btn.h);
    }

    bool currPressed = input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

    if (currPressed && !m_mousePressed) {
        for (auto& btn : m_buttons) {
            if (btn.hovered && btn.callback) {
                btn.callback();
                break;
            }
        }
    }

    m_mousePressed = currPressed;
}

void Menu::render(f32 screenWidth, f32 screenHeight) {
    if (!m_open || !m_shader || !m_font) return;

    setupButtons(screenWidth, screenHeight);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader->use();

    m_shader->setInt("uTexture", 0);

    glm::mat4 proj = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
    m_shader->setMat4("uProjection", proj);

    // Semi-transparent overlay
    renderQuad(0.0f, 0.0f, screenWidth, screenHeight,
               glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));

    glBindTexture(GL_TEXTURE_2D, m_font->textureId());

    // Title "VoxelCraft"
    glm::vec4 titleColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_shader->setVec4("uColor", titleColor);
    m_font->renderText("VoxelCraft",
                       screenWidth * 0.5f - 5.0f * 9.0f,
                       screenHeight * 0.5f - 70.0f,
                       1.5f);

    // Buttons
    for (const auto& btn : m_buttons) {
        // Background
        glm::vec4 bgColor = btn.hovered
            ? glm::vec4(0.3f, 0.5f, 0.8f, 0.9f)
            : glm::vec4(0.2f, 0.3f, 0.5f, 0.8f);
        renderQuad(btn.x, btn.y, btn.w, btn.h, bgColor);

        // Text
        m_shader->setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        f32 textScale = 2.0f;
        f32 charW = 9.0f * textScale;
        f32 textW = static_cast<f32>(btn.label.size()) * charW;
        m_font->renderText(btn.label,
                           btn.x + btn.w * 0.5f - textW * 0.5f,
                           btn.y + btn.h * 0.5f - 6.0f * textScale,
                           textScale);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Menu::renderQuad(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    f32 verts[] = {
        x,   y,   0.0f, 0.0f,
        x+w, y,   1.0f, 0.0f,
        x+w, y+h, 1.0f, 1.0f,
        x,   y+h, 0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, m_whiteTex);
    m_shader->setVec4("uColor", color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
