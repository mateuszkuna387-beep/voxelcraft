#include "Shader.h"

#include <glad/gl.h>

#include <fstream>
#include <iostream>
#include <sstream>

Shader::~Shader() {
    if (m_id) {
        glDeleteProgram(m_id);
    }
}

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return {};
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

u32 Shader::compile(const std::string& source, u32 type) {
    const char* src = source.c_str();
    u32 shader = glCreateShader(type);
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

    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        return false;
    }

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

i32 Shader::uniformLocation(const std::string& name) const {
    return glGetUniformLocation(m_id, name.c_str());
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(uniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(uniformLocation(name), 1, &value[0]);
}

void Shader::setInt(const std::string& name, i32 value) const {
    glUniform1i(uniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, f32 value) const {
    glUniform1f(uniformLocation(name), value);
}
