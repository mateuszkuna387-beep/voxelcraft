#pragma once

#include <cstdint>
#include <array>
#include <functional>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using BlockID = u16;

struct BlockCoord {
    i32 x, y, z;
};

struct ChunkCoord {
    i32 x, z;
};

constexpr bool operator==(const ChunkCoord& a, const ChunkCoord& b) {
    return a.x == b.x && a.z == b.z;
}

constexpr bool operator!=(const ChunkCoord& a, const ChunkCoord& b) {
    return !(a == b);
}

namespace std {
    template<>
    struct hash<ChunkCoord> {
        size_t operator()(const ChunkCoord& c) const noexcept {
            return hash<i32>()(c.x) ^ (hash<i32>()(c.z) << 1);
        }
    };
}
