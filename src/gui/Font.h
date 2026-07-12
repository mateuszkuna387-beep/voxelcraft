#pragma once

#include <string>
#include <unordered_map>
#include <array>

#include "core/Types.h"

class Font {
public:
    Font();
    ~Font();

    bool init();
    void destroy();

    void renderText(const std::string& text, f32 x, f32 y, f32 scale);

    u32 textureId() const { return m_texture; }

private:
    struct GlyphPos {
        i32 x, y, w, h;
    };

    void buildAtlas();
    GlyphPos glyphForCodepoint(char32_t cp) const;

    u32 m_texture = 0;
    u32 m_vao = 0;
    u32 m_vbo = 0;

    static constexpr i32 FONT_W = 8;
    static constexpr i32 FONT_H = 12;
    static constexpr i32 ATLAS_COLS = 16;
    static constexpr i32 ATLAS_ROWS = 16;
    static constexpr i32 ATLAS_W = ATLAS_COLS * (FONT_W + 1) + 1;
    static constexpr i32 ATLAS_H = ATLAS_ROWS * (FONT_H + 1) + 1;

    std::unordered_map<char32_t, std::array<u8, 12>> m_glyphPixels;
    std::unordered_map<char32_t, GlyphPos> m_glyphMap;
    u8 m_atlasData[ATLAS_H][ATLAS_W]{};
};
