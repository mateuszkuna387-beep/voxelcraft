#include "World.h"
#include "Block.h"
#include "core/Constants.h"
#include "rendering/Renderer.h"
#include "rendering/Camera.h"

#include <cmath>
#include <cfloat>

World::World() {
    m_generator = new TerrainGenerator();
}

World::~World() {
    for (auto& [coord, chunk] : m_chunks) {
        delete chunk;
    }
    delete m_generator;
}

bool World::inBounds(i32 x, i32 y, i32 z) {
    return x >= 0 && x < WORLD_SIZE_X &&
           y >= 0 && y < WORLD_SIZE_Y &&
           z >= 0 && z < WORLD_SIZE_Z;
}

void World::generateWorld() {
    i32 numChunksX = (WORLD_SIZE_X + CHUNK_SIZE_X - 1) / CHUNK_SIZE_X;
    i32 numChunksZ = (WORLD_SIZE_Z + CHUNK_SIZE_Z - 1) / CHUNK_SIZE_Z;
    for (i32 cx = 0; cx < numChunksX; ++cx) {
        for (i32 cz = 0; cz < numChunksZ; ++cz) {
            loadChunk(cx, cz);
        }
    }

    for (auto& [coord, chunk] : m_chunks) {
        if (!chunk->isEmpty()) {
            chunk->generateMesh();
            chunk->uploadMesh();
        }
    }
}

void World::render(Renderer* renderer, Camera* camera) {
    (void)renderer;
    (void)camera;

    for (auto& [coord, chunk] : m_chunks) {
        if (chunk->meshDirty() && !chunk->isEmpty()) {
            chunk->generateMesh();
            chunk->uploadMesh();
        }
        chunk->render();
    }
}

void World::setBlock(i32 x, i32 y, i32 z, BlockID id) {
    if (!inBounds(x, y, z)) return;

    i32 cx = x >> 4;
    i32 cz = z >> 4;

    auto it = m_chunks.find({ cx, cz });
    if (it != m_chunks.end()) {
        i32 lx = x & 15;
        i32 lz = z & 15;
        it->second->setBlock(lx, y, lz, id);
    }
}

BlockID World::getBlock(i32 x, i32 y, i32 z) const {
    if (!inBounds(x, y, z)) return BLOCK_AIR;

    i32 cx = x >> 4;
    i32 cz = z >> 4;

    auto it = m_chunks.find({ cx, cz });
    if (it != m_chunks.end()) {
        i32 lx = x & 15;
        i32 lz = z & 15;
        return it->second->getBlock(lx, y, lz);
    }
    return BLOCK_AIR;
}

void World::loadChunk(i32 cx, i32 cz) {
    if (m_chunks.count({ cx, cz })) return;
    auto* chunk = new Chunk(cx, cz);
    m_generator->generate(chunk);
    m_chunks[{cx, cz}] = chunk;
}

void World::unloadChunk(i32 cx, i32 cz) {
    auto it = m_chunks.find({ cx, cz });
    if (it != m_chunks.end()) {
        delete it->second;
        m_chunks.erase(it);
    }
}

Chunk* World::getChunk(i32 cx, i32 cz) {
    auto it = m_chunks.find({ cx, cz });
    return it != m_chunks.end() ? it->second : nullptr;
}

bool World::raycast(const glm::vec3& origin, const glm::vec3& dir,
                    f32 maxDist, i32& hitX, i32& hitY, i32& hitZ,
                    i32& prevX, i32& prevY, i32& prevZ) {
    constexpr f32 EPS = 0.0005f;
    f32 x = origin.x + dir.x * EPS;
    f32 y = origin.y + dir.y * EPS;
    f32 z = origin.z + dir.z * EPS;

    i32 bx = static_cast<i32>(std::floor(x));
    i32 by = static_cast<i32>(std::floor(y));
    i32 bz = static_cast<i32>(std::floor(z));
    prevX = bx; prevY = by; prevZ = bz;

    i32 stepX = (dir.x > 0) ? 1 : -1;
    i32 stepY = (dir.y > 0) ? 1 : -1;
    i32 stepZ = (dir.z > 0) ? 1 : -1;

    f32 tDeltaX = (dir.x != 0) ? std::fabs(1.0f / dir.x) : FLT_MAX;
    f32 tDeltaY = (dir.y != 0) ? std::fabs(1.0f / dir.y) : FLT_MAX;
    f32 tDeltaZ = (dir.z != 0) ? std::fabs(1.0f / dir.z) : FLT_MAX;

    f32 tMaxX = (dir.x != 0)
        ? ((dir.x > 0 ? static_cast<f32>(bx) + 1.0f : static_cast<f32>(bx)) - x) / dir.x
        : FLT_MAX;
    f32 tMaxY = (dir.y != 0)
        ? ((dir.y > 0 ? static_cast<f32>(by) + 1.0f : static_cast<f32>(by)) - y) / dir.y
        : FLT_MAX;
    f32 tMaxZ = (dir.z != 0)
        ? ((dir.z > 0 ? static_cast<f32>(bz) + 1.0f : static_cast<f32>(bz)) - z) / dir.z
        : FLT_MAX;

    for (i32 i = 0; i < 100; ++i) {
        if (inBounds(bx, by, bz)) {
            BlockID id = getBlock(bx, by, bz);
            if (id != BLOCK_AIR && Block::get(id).solid) {
                hitX = bx; hitY = by; hitZ = bz;
                // prevX, prevY, prevZ already hold the previous block position from line 146
                return true;
            }
        }

        prevX = bx; prevY = by; prevZ = bz;

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                if (tMaxX > maxDist) return false;
                bx += stepX;
                tMaxX += tDeltaX;
            } else {
                if (tMaxZ > maxDist) return false;
                bz += stepZ;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                if (tMaxY > maxDist) return false;
                by += stepY;
                tMaxY += tDeltaY;
            } else {
                if (tMaxZ > maxDist) return false;
                bz += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
    }
    return false;
}
