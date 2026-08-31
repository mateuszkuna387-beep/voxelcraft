#pragma once

#include <glm/glm.hpp>

#include "core/Types.h"
#include "Inventory.h"

class World;

class Player {
public:
    Player();

    void move(const glm::vec3& direction, f32 dt);
    void update(f32 dt, World* world);

    const glm::vec3& position() const { return m_position; }
    const glm::vec3& rotation() const { return m_rotation; }

    void setPosition(const glm::vec3& pos) { m_position = pos; }
    void setRotation(const glm::vec3& rot) { m_rotation = rot; }
    bool isOnGround() const { return m_onGround; }

    Inventory& inventory() { return m_inventory; }
    const Inventory& inventory() const { return m_inventory; }

    void startBreak(BlockCoord target);
    void updateBreak(f32 dt, World* world);
    void cancelBreak();
    bool isBreaking() const { return m_isBreaking; }
    f32 breakProgress() const { return m_isBreaking ? (m_breakTimer / m_breakDuration) : 0.0f; }
    BlockCoord breakingBlock() const { return m_breakingBlock; }

private:
    void applyGravity(f32 dt);
    void collideAxis(i32 axis, World* world);

    glm::vec3 m_position{ 0.0f, 64.0f, 0.0f };
    glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_velocity{ 0.0f, 0.0f, 0.0f };

    f32 m_speed = 4.5f;
    f32 m_jumpSpeed = 8.0f;
    bool m_onGround = false;

    Inventory m_inventory;

    bool m_isBreaking = false;
    BlockCoord m_breakingBlock{};
    f32 m_breakTimer = 0.0f;
    f32 m_breakDuration = BASE_BREAK_TIME;
};
