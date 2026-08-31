#pragma once

#include <unordered_map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "core/Types.h"

class Window;
class CameraResumeTest;

class Input {
public:
    void init(Window* window);
    bool isKeyPressed(i32 key) const;
    bool isKeyHeld(i32 key) const;
    void getMouseDelta(f64& dx, f64& dy);
    void getMousePosition(f64& x, f64& y) const;
    bool isMouseButtonPressed(i32 button) const;
    void resetMouseDelta();

    void setMouseDeltaForTesting(f64 dx, f64 dy);

private:
    bool m_testMode = false;
    f64 m_testDeltaX = 0.0;
    f64 m_testDeltaY = 0.0;
    Window* m_window = nullptr;
    f64 m_lastMouseX = 0.0;
    f64 m_lastMouseY = 0.0;
    bool m_firstMouse = true;
};
