#include "Chunk.h"
#include "Block.h"

#include <glm/glm.hpp>

Chunk::Chunk(i32 x, i32 z) {
    m_coord = { x, z };
    m_blocks.fill(BLOCK_AIR);
}

void Chunk::setBlock(i32 x, i32 y, i32 z, BlockID id) {
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z) return;

    m_blocks[index(x, y, z)] = id;
    if (id != BLOCK_AIR) m_empty = false;
    m_meshDirty = true;
}

BlockID Chunk::getBlock(i32 x, i32 y, i32 z) const {
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z) return BLOCK_AIR;

    return m_blocks[index(x, y, z)];
}

bool Chunk::isEmpty() const { return m_empty; }

i32 Chunk::index(i32 x, i32 y, i32 z) const {
    return (y * CHUNK_SIZE_Z + z) * CHUNK_SIZE_X + x;
}

bool Chunk::shouldRenderFace(i32 x, i32 y, i32 z, i32 dx, i32 dy, i32 dz) const {
    i32 nx = x + dx;
    i32 ny = y + dy;
    i32 nz = z + dz;
    BlockID neighbor = getBlock(nx, ny, nz);
    return Block::get(neighbor).transparent;
}

void Chunk::generateMesh() {
    m_mesh.vertices.clear();
    m_mesh.indices.clear();

    if (m_empty) return;

    // Simplified mesh generation — one quad per visible face
    for (i32 y = 0; y < CHUNK_SIZE_Y; ++y) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; ++z) {
            for (i32 x = 0; x < CHUNK_SIZE_X; ++x) {
                BlockID id = m_blocks[index(x, y, z)];
                if (id == BLOCK_AIR) continue;

                glm::vec3 pos(static_cast<f32>(x),
                              static_cast<f32>(y),
                              static_cast<f32>(z));

                // Check each face
                struct { i32 dx, dy, dz; } dirs[] = {
                    { 0, 0, 1 }, { 0, 0, -1 },
                    { 1, 0, 0 }, { -1, 0, 0 },
                    { 0, 1, 0 }, { 0, -1, 0 },
                };

                for (auto [dx, dy, dz] : dirs) {
                    if (!shouldRenderFace(x, y, z, dx, dy, dz)) continue;

                    u32 baseIdx = static_cast<u32>(m_mesh.vertices.size() / 3);

                    // Generate face vertices (simplified)
                    // Placeholder — real implementation would generate proper quads
                }
            }
        }
    }

    m_meshDirty = false;
}

void Chunk::uploadMesh() {
    if (!m_mesh.vertices.empty()) {
        m_mesh.upload();
    }
}

void Chunk::render() const {
    m_mesh.render();
}
