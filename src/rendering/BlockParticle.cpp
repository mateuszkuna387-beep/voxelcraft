#include "BlockParticle.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <algorithm>

#include "world/Block.h"

BlockParticleSystem::~BlockParticleSystem() {
    shutdown();
}

bool BlockParticleSystem::init() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 64 * 7 * sizeof(f32), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), reinterpret_cast<void*>(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), reinterpret_cast<void*>(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    const char* vertSrc = R"(#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in float aLife;
uniform mat4 uMVP;
out vec3 vColor;
out float vLife;
void main(){
    gl_Position = uMVP * vec4(aPos,1.0);
    gl_PointSize = 6.0;
    vColor = aColor;
    vLife = aLife;
})";
    const char* fragSrc = R"(#version 330 core
in vec3 vColor;
in float vLife;
out vec4 FragColor;
void main(){
    float d = length(gl_PointCoord - vec2(0.5));
    if(d > 0.5) discard;
    float alpha = clamp(vLife, 0.0, 1.0);
    FragColor = vec4(vColor * (0.7 + 0.3*alpha), alpha);
})";
    u32 vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertSrc, nullptr);
    glCompileShader(vs);
    u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragSrc, nullptr);
    glCompileShader(fs);
    m_shaderId = glCreateProgram();
    glAttachShader(m_shaderId, vs);
    glAttachShader(m_shaderId, fs);
    glLinkProgram(m_shaderId);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void BlockParticleSystem::shutdown() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_shaderId) glDeleteProgram(m_shaderId);
    m_vao = 0; m_vbo = 0; m_shaderId = 0;
    m_particles.clear();
}

void BlockParticleSystem::spawn(const glm::vec3& blockCenter, BlockID blockId, i32 count) {
    const BlockData& data = Block::get(blockId);
    glm::vec3 baseColor = data.sideColor;
    if (blockId == BLOCK_GRASS) baseColor = data.topColor;

    static std::mt19937 rng(12345);
    std::uniform_real_distribution<f32> distPos(-0.4f, 0.4f);
    std::uniform_real_distribution<f32> distVel(-2.0f, 2.0f);
    std::uniform_real_distribution<f32> distUp(1.0f, 3.0f);
    std::uniform_real_distribution<f32> distLife(0.5f, 1.0f);

    for (i32 i = 0; i < count; ++i) {
        BlockParticle p;
        p.position = blockCenter + glm::vec3(distPos(rng), distPos(rng), distPos(rng));
        p.velocity = glm::vec3(distVel(rng) * 0.5f, distUp(rng), distVel(rng) * 0.5f);
        p.color = baseColor + glm::vec3(distPos(rng) * 0.1f);
        p.color = glm::clamp(p.color, glm::vec3(0.0f), glm::vec3(1.0f));
        p.life = distLife(rng);
        p.maxLife = p.life;
        p.size = 0.1f;
        m_particles.push_back(p);
    }
}

void BlockParticleSystem::update(f32 dt) {
    constexpr f32 GRAVITY = -9.8f;
    for (auto& p : m_particles) {
        if (p.life <= 0.0f) continue;
        p.life -= dt;
        p.velocity.y += GRAVITY * dt * 0.5f;
        p.position += p.velocity * dt;
        p.velocity *= 0.98f;
    }
    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
        [](const BlockParticle& p){ return p.life <= 0.0f; }), m_particles.end());
}

void BlockParticleSystem::render(const glm::mat4& mvp) {
    if (m_particles.empty() || m_shaderId == 0) return;

    std::vector<f32> data;
    data.reserve(m_particles.size() * 7);
    for (auto& p : m_particles) {
        if (p.life <= 0.0f) continue;
        float lifeNorm = p.life / p.maxLife;
        data.push_back(p.position.x);
        data.push_back(p.position.y);
        data.push_back(p.position.z);
        data.push_back(p.color.r);
        data.push_back(p.color.g);
        data.push_back(p.color.b);
        data.push_back(lifeNorm);
    }
    if (data.empty()) return;

    glUseProgram(m_shaderId);
    i32 loc = glGetUniformLocation(m_shaderId, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(f32), data.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, static_cast<i32>(data.size() / 7));

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

size_t BlockParticleSystem::aliveCount() const {
    return m_particles.size();
}

void BlockParticleSystem::clear() {
    m_particles.clear();
}
