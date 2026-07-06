#pragma once

#include "core/Types.h"

class Camera;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void clear();
    void beginFrame(Camera* camera);

private:
    f32 m_clearColor[4] = { 0.5f, 0.7f, 1.0f, 1.0f };
};
