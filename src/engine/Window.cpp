#include "Window.h"

#include <glad/gl.h>

#include <iostream>

bool Window::init(u32 width, u32 height, const char* title, bool fullscreen) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    }

    m_window = glfwCreateWindow(static_cast<i32>(width),
                                 static_cast<i32>(height),
                                 title, monitor, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        return false;
    }

    glfwSwapInterval(1); // vsync on

    m_width = width;
    m_height = height;

    return true;
}

void Window::destroy() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Window::pollEvents() { glfwPollEvents(); }
void Window::swapBuffers() { glfwSwapBuffers(m_window); }
bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }
void Window::setCursorMode(i32 mode) { glfwSetInputMode(m_window, GLFW_CURSOR, mode); }
GLFWwindow* Window::handle() const { return m_window; }
