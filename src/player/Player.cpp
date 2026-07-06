#include "Player.h"
#include "world/World.h"
#include "core/Constants.h"

Player::Player() {
    m_position = glm::vec3(0.0f, 64.0f, 0.0f);
}

void Player::move(const glm::vec3& direction, f32 dt) {
    glm::vec3 forward(-sin(m_rotation.y), 0.0f, -cos(m_rotation.y));
    glm::vec3 right(cos(m_rotation.y), 0.0f, -sin(m_rotation.y));

    glm::vec3 movement = forward * direction.z + right * direction.x;
    movement.y = direction.y;

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
    }

    m_velocity.x = movement.x * m_speed;
    m_velocity.z = movement.z * m_speed;

    if (direction.y > 0.0f && m_onGround) {
        m_velocity.y = m_jumpSpeed;
        m_onGround = false;
    }
}

void Player::update(f32 dt, World* world) {
    applyGravity(dt);
    resolveCollisions(world);
    m_position += m_velocity * dt;
}

void Player::applyGravity(f32 dt) {
    if (!m_onGround) {
        m_velocity.y += GRAVITY * dt;
    }
}

void Player::resolveCollisions(World* world) {
    (void)world;
    // stub — implement AABB collision with world blocks
    m_onGround = false;
}
