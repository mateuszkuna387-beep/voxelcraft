#include "World.h"
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
    i32 cx = x >> 4;
    i32 cz = z >> 4;
    if (cx < 0) cx--;
    if (cz < 0) cz--;

    auto it = m_chunks.find({ cx, cz });
    if (it != m_chunks.end()) {
        i32 lx = x & 15;
        i32 lz = z & 15;
        if (lx < 0) lx += 16;
        if (lz < 0) lz += 16;
        it->second->setBlock(lx, y, lz, id);
    }
}

BlockID World::getBlock(i32 x, i32 y, i32 z) const {
    i32 cx = x >> 4;
    i32 cz = z >> 4;
    if (cx < 0) cx--;
    if (cz < 0) cz--;

    auto it = m_chunks.find({ cx, cz });
    if (it != m_chunks.end()) {
        i32 lx = x & 15;
        i32 lz = z & 15;
        if (lx < 0) lx += 16;
        if (lz < 0) lz += 16;
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
