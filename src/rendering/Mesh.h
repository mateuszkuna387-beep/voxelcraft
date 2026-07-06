#pragma once

#include <vector>

#include "core/Types.h"

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    void upload();
    void render() const;

    std::vector<f32> vertices;
    std::vector<u32> indices;

private:
    u32 m_vao = 0;
    u32 m_vbo = 0;
    u32 m_ebo = 0;
    u32 m_indexCount = 0;
};
