#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

inline AABB blockAABB(const glm::vec3& pos) {
    return AABB{ pos, pos + glm::vec3(1.0f) };
}

inline bool aabbOverlap(const AABB& a, const AABB& b) {
    return (a.min.x < b.max.x && a.max.x > b.min.x) &&
           (a.min.y < b.max.y && a.max.y > b.min.y) &&
           (a.min.z < b.max.z && a.max.z > b.min.z);
}
