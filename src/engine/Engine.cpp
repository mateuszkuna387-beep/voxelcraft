#include "Engine.h"

#include <iostream>

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::init(u32 width, u32 height, const char* title) {
    if (!m_window.init(width, height, title)) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }

    m_input.init(&m_window);
    m_renderer = new Renderer();
    m_camera = new Camera(width, height);
    m_world = new World();
    m_player = new Player();

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
    delete m_player;
    delete m_world;
    delete m_camera;
    delete m_renderer;
    m_window.destroy();
}

void Engine::handleInput(f32 dt) {
    if (m_input.isKeyPressed(GLFW_KEY_ESCAPE)) {
        m_running = false;
    }

    glm::vec3 movement(0.0f);
    if (m_input.isKeyHeld(GLFW_KEY_W)) movement.z -= 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_S)) movement.z += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_A)) movement.x -= 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_D)) movement.x += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_SPACE)) movement.y += 1.0f;
    if (m_input.isKeyHeld(GLFW_KEY_LEFT_SHIFT)) movement.y -= 1.0f;

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
        m_player->move(movement, dt);
    }
}

void Engine::update(f32 dt) {
    m_player->update(dt, m_world);
    m_camera->follow(m_player->position(), m_player->rotation());
}

void Engine::render() {
    m_renderer->clear();
    m_renderer->beginFrame(m_camera);
    m_world->render(m_renderer, m_camera);
}
