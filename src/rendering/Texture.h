#pragma once

#include <string>

#include "core/Types.h"

class Texture {
public:
    Texture() = default;
    ~Texture();

    bool load(const std::string& path);
    void bind(u32 unit = 0) const;
    u32 id() const { return m_id; }

private:
    u32 m_id = 0;
    i32 m_width = 0;
    i32 m_height = 0;
    i32 m_channels = 0;
};
