#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

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
    bool raycast(const glm::vec3& origin, const glm::vec3& dir,
                 f32 maxDist, i32& hitX, i32& hitY, i32& hitZ,
                 i32& prevX, i32& prevY, i32& prevZ);

private:
    std::unordered_map<ChunkCoord, Chunk*> m_chunks;
    TerrainGenerator* m_generator = nullptr;
};
