#pragma once

#include <string>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include "core/Types.h"
#include "rendering/Shader.h"

class Window;
class Input;
class CameraResumeTest;

class Menu {
public:
    Menu();
    ~Menu();

    bool init();
    void destroy();

    void toggle();
    bool isOpen() const { return m_open; }

    void handleInput(Input& input, Window& window);
    void render(f32 screenWidth, f32 screenHeight);

    void onResume(std::function<void()> callback) { m_onResume = callback; }
    void onExit(std::function<void()> callback) { m_onExit = callback; }

    friend class CameraResumeTest;

private:
    struct Button {
        f32 x, y, w, h;
        std::string label;
        bool hovered = false;
        std::function<void()> callback;
    };

    void setupButtons(f32 screenWidth, f32 screenHeight);
    void renderQuad(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color);

    bool m_open = false;
    Shader* m_shader = nullptr;
    class Font* m_font = nullptr;

    u32 m_vao = 0;
    u32 m_vbo = 0;
    u32 m_whiteTex = 0;

    std::vector<Button> m_buttons;
    bool m_mousePressed = false;

    std::function<void()> m_onResume;
    std::function<void()> m_onExit;
};
