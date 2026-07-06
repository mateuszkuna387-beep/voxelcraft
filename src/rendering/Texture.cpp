#include "Texture.h"

#include <glad/gl.h>

Texture::~Texture() {
    if (m_id) {
        glDeleteTextures(1, &m_id);
    }
}

bool Texture::load(const std::string& path) {
    (void)path;
    // stub — implement with stb_image or similar
    return false;
}

void Texture::bind(u32 unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}
