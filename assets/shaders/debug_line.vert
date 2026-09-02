#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vColor;

uniform mat4 uMVP;
uniform vec3 uColor;

void main() {
    vColor = uColor;
    gl_Position = uMVP * vec4(aPos, 1.0);
}