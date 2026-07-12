#include "Engine.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::init(u32 width, u32 height, const char* title) {
    if (!m_window.init(width, height, title)) {
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
    m_menu.destroy();
    delete m_player;
    delete m_world;
    delete m_camera;
    delete m_shader;
    delete m_renderer;
    m_window.destroy();
}

void Engine::handleInput(f32 dt) {
    (void)dt;

    bool currF1 = m_input.isKeyPressed(GLFW_KEY_F1);
    if (currF1 && !m_prevEscape) {
        m_menu.toggle();
        if (m_menu.isOpen()) {
            m_window.setCursorMode(GLFW_CURSOR_NORMAL);
        } else {
            m_window.setCursorMode(GLFW_CURSOR_DISABLED);
        }
    }
    m_prevEscape = currF1;

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
}

void Engine::render() {
    m_renderer->clear();

    m_shader->use();

    glm::mat4 view = m_camera->viewMatrix();
    glm::mat4 proj = m_camera->projectionMatrix();
    glm::mat4 mvp = proj * view;

    m_shader->setMat4("uMVP", mvp);
    m_shader->setVec3("uColor", glm::vec3(0.6f, 0.6f, 0.6f));

    m_world->render(m_renderer, m_camera);

    if (m_menu.isOpen()) {
        m_menu.render(static_cast<f32>(m_window.width()),
                      static_cast<f32>(m_window.height()));
    }
}
