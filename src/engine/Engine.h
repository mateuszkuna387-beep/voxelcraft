#pragma once

#include <glm/glm.hpp>

#include "Window.h"
#include "Input.h"
#include "rendering/Renderer.h"
#include "rendering/Shader.h"
#include "rendering/Camera.h"
#include "rendering/BlockParticle.h"
#include "world/World.h"
#include "player/Player.h"
#include "gui/Menu.h"
#include "gui/Font.h"

class Engine {
public:
    Engine();
    ~Engine();

    bool init(u32 width, u32 height, const char* title);
    void run();
    void shutdown();

    void handleInput(f32 dt);
    void update(f32 dt);

    Window& window() { return m_window; }
    const Window& window() const { return m_window; }
    Input& input() { return m_input; }
    const Input& input() const { return m_input; }
    Camera* camera() { return m_camera; }
    const Camera* camera() const { return m_camera; }
    Player* player() { return m_player; }
    const Player* player() const { return m_player; }
    Menu& menu() { return m_menu; }
    const Menu& menu() const { return m_menu; }
    World* world() { return m_world; }
    const World* world() const { return m_world; }

    bool hasBlockTarget() const { return m_hasTarget; }
    glm::ivec3 targetBlock() const { return { m_targetX, m_targetY, m_targetZ }; }
    glm::ivec3 placeBlock() const { return { m_placeX, m_placeY, m_placeZ }; }
    bool debugMode() const { return m_debugMode; }

private:
    void render();

    void updateBlockTarget();
    void updateBlockBreaking(f32 dt);
    void updateBlockPlacing();
    void renderBlockHighlight();
    void renderBreakOverlay();
    void renderDebugBeam();
    void renderDebugText(f32 screenWidth, f32 screenHeight);
    void renderCrosshair(f32 screenWidth, f32 screenHeight);
    void renderQuad(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color);

    Window m_window;
    Input m_input;
    Renderer* m_renderer = nullptr;
    Shader* m_shader = nullptr;
    Shader* m_debugLineShader = nullptr;
    Camera* m_camera = nullptr;
    World* m_world = nullptr;
    Player* m_player = nullptr;
    Menu m_menu;

    bool m_running = false;
    bool m_prevEscape = false;
    bool m_prevLMB = false;
    bool m_prevRMB = false;
    bool m_prevDebug = false;
    f32 m_lastFrameTime = 0.0f;

    // Camera state for pause/resume
    glm::vec3 m_savedCameraPosition{ 0.0f };
    glm::vec3 m_savedCameraRotation{ 0.0f };
    bool m_hasSavedCameraState = false;
    bool m_skipNextCameraFollow = false;

    // Block targeting and breaking
    bool m_hasTarget = false;
    i32 m_targetX = 0, m_targetY = 0, m_targetZ = 0;
    i32 m_placeX = 0, m_placeY = 0, m_placeZ = 0;

    bool m_isBreaking = false;
    i32 m_breakX = 0, m_breakY = 0, m_breakZ = 0;
    BlockID m_breakBlockType = BLOCK_AIR;
    f32 m_breakTimer = 0.0f;
    f32 m_breakDuration = 0.0f;

    // Overlay rendering
    u32 m_overlayVAO = 0;
    u32 m_overlayVBO = 0;
    u32 m_overlayEBO = 0;
    u32 m_overlayIndexCount = 0;
    u32 m_wireframeVAO = 0;
    u32 m_wireframeVBO = 0;

    // Crosshair
    u32 m_crosshairVAO = 0;
    u32 m_crosshairVBO = 0;
    Shader* m_guiShader = nullptr;
    u32 m_whiteTex = 0;

    // Debug mode
    bool m_debugMode = true;
    u32 m_debugLineVAO = 0;
    u32 m_debugLineVBO = 0;
    Font m_font;

    BlockParticleSystem m_particles;
    f32 m_particleSpawnTimer = 0.0f;
};
