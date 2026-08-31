#include "Engine.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <cmath>

#include "world/Block.h"

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::init(u32 width, u32 height, const char* title) {
    if (!m_window.init(width, height, title, true)) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }

    m_input.init(&m_window);

    if (!m_menu.init()) {
        std::cerr << "Failed to initialize menu" << std::endl;
        return false;
    }

    m_menu.onResume([this]() {
        m_menu.toggle();
        m_window.setCursorMode(GLFW_CURSOR_DISABLED);
    });

    m_menu.onExit([this]() {
        m_running = false;
    });

    m_guiShader = new Shader();
    if (!m_guiShader->load("assets/shaders/gui.vert", "assets/shaders/gui.frag")) {
        std::cerr << "Failed to load GUI shaders" << std::endl;
        return false;
    }

    u8 whitePixel[4] = { 255, 255, 255, 255 };
    glGenTextures(1, &m_whiteTex);
    glBindTexture(GL_TEXTURE_2D, m_whiteTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenVertexArrays(1, &m_crosshairVAO);
    glGenBuffers(1, &m_crosshairVBO);
    glBindVertexArray(m_crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_crosshairVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32),
                          reinterpret_cast<void*>(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    m_renderer = new Renderer();
    m_shader = new Shader();
    if (!m_shader->load("assets/shaders/block.vert", "assets/shaders/block.frag")) {
        std::cerr << "Failed to load shaders" << std::endl;
        return false;
    }

    m_camera = new Camera(width, height);
    m_camera->setPosition(glm::vec3(5.0f, 14.0f, 18.0f));

    m_world = new World();
    m_world->generateWorld();

    m_player = new Player();
    m_player->setPosition(glm::vec3(50.0f, 70.0f, 90.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    // Create overlay cube VAO (unit cube from 0,0,0 to 1,1,1)
    {
        // 6 faces, 4 verts each = 24 verts (position + faceIndex)
        f32 overlayVerts[] = {
            // +Z face (face index 0)
            0,0,1, 0,  1,0,1, 0,  1,1,1, 0,  0,1,1, 0,
            // -Z face (face index 1)
            1,0,0, 1,  0,0,0, 1,  0,1,0, 1,  1,1,0, 1,
            // +X face (face index 2)
            1,0,0, 2,  1,0,1, 2,  1,1,1, 2,  1,1,0, 2,
            // -X face (face index 3)
            0,0,1, 3,  0,0,0, 3,  0,1,0, 3,  0,1,1, 3,
            // +Y face (face index 4)
            0,1,1, 4,  1,1,1, 4,  1,1,0, 4,  0,1,0, 4,
            // -Y face (face index 5)
            0,0,0, 5,  1,0,0, 5,  1,0,1, 5,  0,0,1, 5,
        };
        u32 overlayIndices[] = {
            0,1,2, 0,2,3,
            4,5,6, 4,6,7,
            8,9,10, 8,10,11,
            12,13,14, 12,14,15,
            16,17,18, 16,18,19,
            20,21,22, 20,22,23,
        };

        glGenVertexArrays(1, &m_overlayVAO);
        glGenBuffers(1, &m_overlayVBO);
        glGenBuffers(1, &m_overlayEBO);

        glBindVertexArray(m_overlayVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_overlayVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(overlayVerts), overlayVerts, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_overlayEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(overlayIndices), overlayIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(f32),
                              reinterpret_cast<void*>(3 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        m_overlayIndexCount = 36;
    }

    // Create wireframe cube VAO (12 edges)
    {
        f32 wireVerts[] = {
            // bottom face
            0,0,0, 1,0,0,  1,0,0, 1,1,0,  1,1,0, 0,1,0,  0,1,0, 0,0,0,
            // top face
            0,0,1, 1,0,1,  1,0,1, 1,1,1,  1,1,1, 0,1,1,  0,1,1, 0,0,1,
            // verticals
            0,0,0, 0,0,1,  1,0,0, 1,0,1,  1,1,0, 1,1,1,  0,1,0, 0,1,1,
        };

        glGenVertexArrays(1, &m_wireframeVAO);
        glGenBuffers(1, &m_wireframeVBO);

        glBindVertexArray(m_wireframeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_wireframeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wireVerts), wireVerts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), nullptr);
        glEnableVertexAttribArray(0);
    }

    m_running = true;
    m_lastFrameTime = static_cast<f32>(glfwGetTime());

    return true;
}

void Engine::run() {
    while (m_running && !m_window.shouldClose()) {
        f32 currentTime = static_cast<f32>(glfwGetTime());
        f32 dt = currentTime - m_lastFrameTime;
        m_lastFrameTime = currentTime;

        m_window.pollEvents();
        handleInput(dt);
        update(dt);
        render();
        m_window.swapBuffers();
    }
}

void Engine::shutdown() {
    if (m_overlayVAO) glDeleteVertexArrays(1, &m_overlayVAO);
    if (m_overlayVBO) glDeleteBuffers(1, &m_overlayVBO);
    if (m_overlayEBO) glDeleteBuffers(1, &m_overlayEBO);
    if (m_wireframeVAO) glDeleteVertexArrays(1, &m_wireframeVAO);
    if (m_wireframeVBO) glDeleteBuffers(1, &m_wireframeVBO);

    if (m_crosshairVAO) glDeleteVertexArrays(1, &m_crosshairVAO);
    if (m_crosshairVBO) glDeleteBuffers(1, &m_crosshairVBO);
    if (m_whiteTex) glDeleteTextures(1, &m_whiteTex);

    m_menu.destroy();
    delete m_player;
    delete m_world;
    delete m_camera;
    delete m_shader;
    delete m_guiShader;
    delete m_renderer;
    m_window.destroy();
}

void Engine::handleInput(f32 dt) {
    (void)dt;

    bool currEscape = m_input.isKeyPressed(GLFW_KEY_ESCAPE);
    if (currEscape && !m_prevEscape) {
        m_menu.toggle();
        if (m_menu.isOpen()) {
            m_window.setCursorMode(GLFW_CURSOR_NORMAL);
        } else {
            m_window.setCursorMode(GLFW_CURSOR_DISABLED);
        }
    }
    m_prevEscape = currEscape;

    if (m_menu.isOpen()) {
        m_menu.handleInput(m_input, m_window);
        return;
    }

    f64 dx, dy;
    m_input.getMouseDelta(dx, dy);
    glm::vec3 rot = m_player->rotation();
    rot.y += static_cast<f32>(dx) * MOUSE_SENSITIVITY;
    rot.x += static_cast<f32>(dy) * MOUSE_SENSITIVITY;
    rot.x = glm::clamp(rot.x, -glm::half_pi<f32>(), glm::half_pi<f32>());
    m_player->setRotation(rot);

    glm::vec3 movement(0.0f);
    if (m_input.isKeyHeld(GLFW_KEY_W)) movement.z += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_S)) movement.z -= 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_A)) movement.x -= 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_D)) movement.x += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_SPACE)) movement.y += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_LEFT_SHIFT)) movement.y -= 1.0f;

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
    }
    m_player->move(movement, dt);
}

void Engine::update(f32 dt) {
    if (m_menu.isOpen()) return;

    m_player->update(dt, m_world);
    m_camera->follow(m_player->position(), m_player->rotation());

    updateBlockTarget();
    updateBlockBreaking(dt);
}

void Engine::updateBlockTarget() {
    glm::vec3 camPos = m_camera->position();
    glm::vec3 rot = m_player->rotation();

    glm::vec3 dir;
    dir.x = std::sin(rot.y) * std::cos(rot.x);
    dir.y = std::sin(rot.x);
    dir.z = -std::cos(rot.y) * std::cos(rot.x);
    dir = glm::normalize(dir);

    m_hasTarget = m_world->raycast(camPos, dir, RAYCAST_DISTANCE,
                                   m_targetX, m_targetY, m_targetZ,
                                   m_placeX, m_placeY, m_placeZ);
}

void Engine::updateBlockBreaking(f32 dt) {
    bool lmbHeld = m_input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

    if (!m_hasTarget || !lmbHeld) {
        if (m_isBreaking) {
            m_isBreaking = false;
            m_breakTimer = 0.0f;
        }
        return;
    }

    if (!m_isBreaking) {
        m_isBreaking = true;
        m_breakX = m_targetX;
        m_breakY = m_targetY;
        m_breakZ = m_targetZ;
        m_breakBlockType = m_world->getBlock(m_targetX, m_targetY, m_targetZ);
        const BlockData& data = Block::get(m_breakBlockType);
        if (data.hardness < 0.0f) {
            m_isBreaking = false;
            return;
        }
        m_breakDuration = BASE_BREAK_TIME * data.hardness;
        m_breakTimer = 0.0f;
    }

    if (m_breakX != m_targetX || m_breakY != m_targetY || m_breakZ != m_targetZ) {
        m_breakX = m_targetX;
        m_breakY = m_targetY;
        m_breakZ = m_targetZ;
        m_breakBlockType = m_world->getBlock(m_targetX, m_targetY, m_targetZ);
        const BlockData& data = Block::get(m_breakBlockType);
        if (data.hardness < 0.0f) {
            m_isBreaking = false;
            return;
        }
        m_breakDuration = BASE_BREAK_TIME * data.hardness;
        m_breakTimer = 0.0f;
    }

    m_breakTimer += dt;

    if (m_breakTimer >= m_breakDuration) {
        m_world->setBlock(m_breakX, m_breakY, m_breakZ, BLOCK_AIR);
        m_player->inventory().addBlock(m_breakBlockType, 1);
        m_isBreaking = false;
        m_breakTimer = 0.0f;
    }
}

void Engine::render() {
    m_renderer->clear();

    // ---- Render world ----
    m_shader->use();
    m_shader->setFloat("uOverlay", 0.0f);

    glm::mat4 view = m_camera->viewMatrix();
    glm::mat4 proj = m_camera->projectionMatrix();
    glm::mat4 mvp = proj * view;

    m_shader->setMat4("uMVP", mvp);

    m_world->render(m_renderer, m_camera);

    // ---- Render block highlight and break overlay ----
    if (m_hasTarget && !m_menu.isOpen()) {
        // Only highlight if player is within 3 blocks
        glm::vec3 playerPos = m_player->position();
        glm::vec3 targetPos(static_cast<f32>(m_targetX) + 0.5f,
                            static_cast<f32>(m_targetY) + 0.5f,
                            static_cast<f32>(m_targetZ) + 0.5f);
        f32 dist = glm::length(playerPos - targetPos);
        if (dist <= BLOCK_HIGHLIGHT_DISTANCE) {
            renderBlockHighlight();
        }
        if (m_isBreaking) {
            renderBreakOverlay();
        }
    }

    // ---- Render crosshair (when menu is closed) ----
    if (!m_menu.isOpen()) {
        renderCrosshair(static_cast<f32>(m_window.width()),
                        static_cast<f32>(m_window.height()));
    }

    // ---- Render menu ----
    if (m_menu.isOpen()) {
        m_menu.render(static_cast<f32>(m_window.width()),
                      static_cast<f32>(m_window.height()));
    }
}

void Engine::renderCrosshair(f32 screenWidth, f32 screenHeight) {
    if (!m_guiShader || !m_whiteTex) return;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_guiShader->use();
    m_guiShader->setInt("uTexture", 0);

    glm::mat4 proj = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
    m_guiShader->setMat4("uProjection", proj);

    f32 centerX = screenWidth * 0.5f;
    f32 centerY = screenHeight * 0.5f;
    f32 size = 10.0f;
    f32 thickness = 2.0f;
    f32 gap = 4.0f;

    glBindTexture(GL_TEXTURE_2D, m_whiteTex);
    m_guiShader->setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));

    // Horizontal line (left)
    renderQuad(centerX - gap - size, centerY - thickness * 0.5f, size, thickness, glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
    // Horizontal line (right)
    renderQuad(centerX + gap, centerY - thickness * 0.5f, size, thickness, glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
    // Vertical line (top)
    renderQuad(centerX - thickness * 0.5f, centerY - gap - size, thickness, size, glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
    // Vertical line (bottom)
    renderQuad(centerX - thickness * 0.5f, centerY + gap, thickness, size, glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Engine::renderQuad(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color) {
    glBindVertexArray(m_crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_crosshairVBO);

    f32 verts[] = {
        x,   y,   0.0f, 0.0f,
        x+w, y,   1.0f, 0.0f,
        x+w, y+h, 1.0f, 1.0f,
        x,   y+h, 0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    m_guiShader->setVec4("uColor", color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Engine::renderBlockHighlight() {
    glm::mat4 view = m_camera->viewMatrix();
    glm::mat4 proj = m_camera->projectionMatrix();

    glm::mat4 model = glm::translate(glm::mat4(1.0f),
        glm::vec3(static_cast<f32>(m_targetX), static_cast<f32>(m_targetY), static_cast<f32>(m_targetZ)));
    // Scale slightly larger to avoid z-fighting with world block
    model = glm::scale(model, glm::vec3(1.01f));
    glm::mat4 mvp = proj * view * model;

    m_shader->use();
    m_shader->setMat4("uMVP", mvp);
    m_shader->setFloat("uOverlay", 0.0f);
    m_shader->setFloat("uHighlight", 1.0f);

    glBindVertexArray(m_overlayVAO);
    glDrawElements(GL_TRIANGLES, m_overlayIndexCount, GL_UNSIGNED_INT, nullptr);

    m_shader->setFloat("uHighlight", 0.0f);
}

void Engine::renderBreakOverlay() {
    f32 progress = m_breakTimer / m_breakDuration;
    if (progress > 1.0f) progress = 1.0f;

    glm::mat4 view = m_camera->viewMatrix();
    glm::mat4 proj = m_camera->projectionMatrix();

    glm::mat4 model = glm::translate(glm::mat4(1.0f),
        glm::vec3(static_cast<f32>(m_breakX), static_cast<f32>(m_breakY), static_cast<f32>(m_breakZ)));
    glm::mat4 mvp = proj * view * model;

    m_shader->use();
    m_shader->setMat4("uMVP", mvp);
    m_shader->setFloat("uOverlay", progress);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(m_overlayVAO);
    glDrawElements(GL_TRIANGLES, m_overlayIndexCount, GL_UNSIGNED_INT, nullptr);

    glDisable(GL_BLEND);
    m_shader->setFloat("uOverlay", 0.0f);
}
