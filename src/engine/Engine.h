#pragma once

#include "Window.h"
#include "Input.h"
#include "rendering/Renderer.h"
#include "rendering/Shader.h"
#include "rendering/Camera.h"
#include "world/World.h"
#include "player/Player.h"
#include "gui/Menu.h"

class Engine {
public:
    Engine();
    ~Engine();

    bool init(u32 width, u32 height, const char* title);
    void run();
    void shutdown();

private:
    void handleInput(f32 dt);
    void update(f32 dt);
    void render();

    Window m_window;
    Input m_input;
    Renderer* m_renderer = nullptr;
    Shader* m_shader = nullptr;
    Camera* m_camera = nullptr;
    World* m_world = nullptr;
    Player* m_player = nullptr;
    Menu m_menu;

    bool m_running = false;
    bool m_prevEscape = false;
    f32 m_lastFrameTime = 0.0f;
};
