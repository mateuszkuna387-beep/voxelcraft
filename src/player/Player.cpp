#include "Player.h"
#include "world/World.h"
#include "world/Block.h"
#include "core/Constants.h"
Player::Player() {
    m_position = glm::vec3(0.0f, 64.0f, 0.0f);
}

void Player::move(const glm::vec3& direction, f32 dt) {
    glm::vec3 forward(sin(m_rotation.y), 0.0f, -cos(m_rotation.y));
    glm::vec3 right(cos(m_rotation.y), 0.0f, sin(m_rotation.y));

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

    if (m_velocity.y < -MAX_FALL_SPEED) {
        m_velocity.y = -MAX_FALL_SPEED;
    }

    m_position.x += m_velocity.x * dt;
    collideAxis(0, world);

    m_position.z += m_velocity.z * dt;
    collideAxis(2, world);

    m_onGround = false;
    m_position.y += m_velocity.y * dt;
    collideAxis(1, world);
    if (m_onGround && m_velocity.y < 0) {
        m_velocity.y = 0;
    }
}

void Player::applyGravity(f32 dt) {
    if (!m_onGround) {
        m_velocity.y += GRAVITY * dt;
    }
}

void Player::collideAxis(i32 axis, World* world) {
    const f32 r = PLAYER_RADIUS;
    const f32 h = PLAYER_HEIGHT;

    f32 minX = m_position.x - r;
    f32 maxX = m_position.x + r;
    f32 minY = m_position.y;
    f32 maxY = m_position.y + h;
    f32 minZ = m_position.z - r;
    f32 maxZ = m_position.z + r;

    i32 sX = static_cast<i32>(floor(minX)) - 1;
    i32 eX = static_cast<i32>(floor(maxX));
    i32 sY = static_cast<i32>(floor(minY)) - 1;
    i32 eY = static_cast<i32>(floor(maxY));
    i32 sZ = static_cast<i32>(floor(minZ)) - 1;
    i32 eZ = static_cast<i32>(floor(maxZ));

    float minBound = -INFINITY;
    float maxBound = INFINITY;

    for (i32 by = sY; by <= eY; ++by) {
        for (i32 bz = sZ; bz <= eZ; ++bz) {
            for (i32 bx = sX; bx <= eX; ++bx) {
                BlockID id = world->getBlock(bx, by, bz);
                if (id == BLOCK_AIR || !Block::get(id).solid) continue;

                bool overlapX = maxX > bx && minX < bx + 1;
                bool overlapY = maxY > by && minY < by + 1;
                bool overlapZ = maxZ > bz && minZ < bz + 1;



                if (axis == 0) {
                    if (!overlapY || !overlapZ) continue;
                    if (m_position.x > bx + 0.5f) {
                        minBound = std::max(minBound, bx + 1.0f + r);
                    } else {
                        maxBound = std::min(maxBound, bx - r);
                    }
                } else if (axis == 1) {
                    if (!overlapX || !overlapZ) continue;
                    if (m_position.y + h * 0.5f > by + 0.5f) {
                        minBound = std::max(minBound, by + 1.0f);
                    } else {
                        maxBound = std::min(maxBound, by - h);
                    }
                } else {
                    if (!overlapX || !overlapY) continue;
                    if (m_position.z > bz + 0.5f) {
                        minBound = std::max(minBound, bz + 1.0f + r);
                    } else {
                        maxBound = std::min(maxBound, bz - r);
                    }
                }
            }
        }
    }

    if (axis == 1) {
        f32 oldY = m_position.y;
        if (minBound <= maxBound) {
            m_position.y = glm::clamp(m_position.y, minBound, maxBound);
        } else if (minBound > -INFINITY) {
            m_position.y = std::max(m_position.y, minBound);
        }
        if (m_position.y > oldY || (minBound > -INFINITY && m_position.y - minBound < 0.001f)) {
            m_onGround = true;
        }
    } else {
        if (minBound <= maxBound) {
            if (axis == 0) {
                m_position.x = glm::clamp(m_position.x, minBound, maxBound);
            } else {
                m_position.z = glm::clamp(m_position.z, minBound, maxBound);
            }
        }
    }
}
