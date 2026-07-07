#pragma once

#include <string>

#include <glm/glm.hpp>

#include "core/Types.h"

class Shader {
public:
    Shader() = default;
    ~Shader();

    bool load(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;

    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

    u32 id() const { return m_id; }

private:
    u32 compile(const std::string& source, u32 type);
    i32 uniformLocation(const std::string& name) const;

    u32 m_id = 0;
};
