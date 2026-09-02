#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "core/Types.h"

struct BlockParticle {
    glm::vec3 position{ 0.0f };
    glm::vec3 velocity{ 0.0f };
    glm::vec3 color{ 0.5f };
    f32 life = 0.0f;
    f32 maxLife = 0.0f;
    f32 size = 0.1f;
};

class BlockParticleSystem {
public:
    BlockParticleSystem() = default;
    ~BlockParticleSystem();

    bool init();
    void shutdown();

    void spawn(const glm::vec3& blockCenter, BlockID blockId, i32 count);
    void update(f32 dt);
    void render(const glm::mat4& mvp);

    size_t aliveCount() const;
    void clear();

private:
    std::vector<BlockParticle> m_particles;
    u32 m_vao = 0;
    u32 m_vbo = 0;
    u32 m_shaderId = 0;
};
