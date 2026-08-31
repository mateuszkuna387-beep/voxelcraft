#include "Input.h"
#include "Window.h"

void Input::init(Window* window) {
    m_window = window;
    glfwSetInputMode(m_window->handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Input::isKeyPressed(i32 key) const {
    return glfwGetKey(m_window->handle(), key) == GLFW_PRESS;
}

bool Input::isKeyHeld(i32 key) const {
    return glfwGetKey(m_window->handle(), key) == GLFW_PRESS;
}

void Input::getMousePosition(f64& x, f64& y) const {
    glfwGetCursorPos(m_window->handle(), &x, &y);
}

bool Input::isMouseButtonPressed(i32 button) const {
    return glfwGetMouseButton(m_window->handle(), button) == GLFW_PRESS;
}

void Input::resetMouseDelta() {
    m_firstMouse = true;
}

void Input::setMouseDeltaForTesting(f64 dx, f64 dy) {
    m_testMode = true;
    m_testDeltaX = dx;
    m_testDeltaY = dy;
}

void Input::getMouseDelta(f64& dx, f64& dy) {
    if (m_testMode) {
        dx = m_testDeltaX;
        dy = m_testDeltaY;
        m_testMode = false;
        return;
    }

    f64 x, y;
    glfwGetCursorPos(m_window->handle(), &x, &y);

    if (m_firstMouse) {
        m_lastMouseX = x;
        m_lastMouseY = y;
        m_firstMouse = false;
    }

    dx = x - m_lastMouseX;
    dy = y - m_lastMouseY;
    m_lastMouseX = x;
    m_lastMouseY = y;
}
