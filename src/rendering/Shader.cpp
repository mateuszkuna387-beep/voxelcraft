#include "Shader.h"

#include <glad/gl.h>
#include <iostream>

Shader::~Shader() {
    if (m_id) {
        glDeleteProgram(m_id);
    }
}

u32 Shader::compile(const std::string& source, u32 type) {
    u32 shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    i32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }

    return shader;
}

bool Shader::load(const std::string& vertexPath, const std::string& fragmentPath) {
    if (m_id) {
        glDeleteProgram(m_id);
    }

    std::string vertexCode, fragmentCode;
    // Read files (simplified)
    vertexCode = "#version 330 core\nlayout(location=0) in vec3 aPos;uniform mat4 uMVP;void main(){gl_Position=uMVP*vec4(aPos,1.0);}";
    fragmentCode = "#version 330 core\nout vec4 FragColor;uniform vec3 uColor;void main(){FragColor=vec4(uColor,1.0);}";

    u32 vs = compile(vertexCode, GL_VERTEX_SHADER);
    u32 fs = compile(fragmentCode, GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    glLinkProgram(m_id);

    i32 success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void Shader::use() const {
    glUseProgram(m_id);
}
