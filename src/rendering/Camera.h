#pragma once

#include <glm/glm.hpp>

#include "core/Types.h"

class Camera {
public:
    Camera(u32 width, u32 height);

    void follow(const glm::vec3& position, const glm::vec3& rotation);

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::vec3 position() const { return m_position; }

    void setPosition(const glm::vec3& pos) { m_position = pos; }

private:
    glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
    f32 m_fov = 75.0f;
    f32 m_near = 0.1f;
    f32 m_far = 1000.0f;
    f32 m_aspectRatio = 16.0f / 9.0f;
};
