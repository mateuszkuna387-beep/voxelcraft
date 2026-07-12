#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "core/Types.h"

class Window {
public:
    Window() = default;
    ~Window() = default;

    bool init(u32 width, u32 height, const char* title);
    void destroy();

    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void setCursorMode(i32 mode);
    GLFWwindow* handle() const;

    u32 width() const { return m_width; }
    u32 height() const { return m_height; }

private:
    GLFWwindow* m_window = nullptr;
    u32 m_width = 0;
    u32 m_height = 0;
};
