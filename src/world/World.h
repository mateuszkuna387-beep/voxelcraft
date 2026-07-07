#pragma once

#include <unordered_map>

#include "core/Types.h"
#include "Chunk.h"
#include "TerrainGenerator.h"

class Renderer;
class Camera;

class World {
public:
    World();
    ~World();

    void generateWorld();
    void render(Renderer* renderer, Camera* camera);
    void setBlock(i32 x, i32 y, i32 z, BlockID id);
    BlockID getBlock(i32 x, i32 y, i32 z) const;

    void loadChunk(i32 cx, i32 cz);
    void unloadChunk(i32 cx, i32 cz);
    Chunk* getChunk(i32 cx, i32 cz);

    static bool inBounds(i32 x, i32 y, i32 z);

private:
    std::unordered_map<ChunkCoord, Chunk*> m_chunks;
    TerrainGenerator* m_generator = nullptr;
};
