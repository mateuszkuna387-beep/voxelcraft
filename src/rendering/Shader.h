#pragma once

#include <string>

#include "core/Types.h"

class Shader {
public:
    Shader() = default;
    ~Shader();

    bool load(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;
    u32 id() const { return m_id; }

private:
    u32 compile(const std::string& source, u32 type);
    u32 m_id = 0;
};
