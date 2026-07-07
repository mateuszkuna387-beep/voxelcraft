#include "World.h"
#include "core/Constants.h"
#include "rendering/Renderer.h"
#include "rendering/Camera.h"

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
