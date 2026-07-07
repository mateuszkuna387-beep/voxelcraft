#include "Camera.h"
#include "core/Constants.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(u32 width, u32 height) {
    m_aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
}

void Camera::follow(const glm::vec3& position, const glm::vec3& rotation) {
    m_position = position + glm::vec3(0.0f, PLAYER_EYE_HEIGHT, 0.0f);
    m_rotation = rotation;
}

glm::mat4 Camera::viewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::rotate(view, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -m_position);
    return view;
}

glm::mat4 Camera::projectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
}
