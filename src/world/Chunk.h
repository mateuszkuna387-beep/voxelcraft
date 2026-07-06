#pragma once

#include <array>
#include <vector>

#include "core/Types.h"
#include "core/Constants.h"
#include "rendering/Mesh.h"

class Chunk {
public:
    Chunk(i32 x, i32 z);

    void setBlock(i32 x, i32 y, i32 z, BlockID id);
    BlockID getBlock(i32 x, i32 y, i32 z) const;
    bool isEmpty() const;

    void generateMesh();
    void uploadMesh();
    void render() const;

    const ChunkCoord& coord() const { return m_coord; }
    bool meshDirty() const { return m_meshDirty; }

private:
    i32 index(i32 x, i32 y, i32 z) const;
    bool shouldRenderFace(i32 x, i32 y, i32 z, i32 dx, i32 dy, i32 dz) const;

    ChunkCoord m_coord;
    std::array<BlockID, CHUNK_VOLUME> m_blocks{};
    Mesh m_mesh;
    bool m_meshDirty = true;
    bool m_empty = true;
};
