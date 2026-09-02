#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aFaceIndex;

out vec3 FragPos;
out float vFaceIndex;
out vec3 WorldPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main() {
    FragPos = aPos;
    vFaceIndex = aFaceIndex;
    WorldPos = (uModel * vec4(aPos, 1.0)).xyz;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
