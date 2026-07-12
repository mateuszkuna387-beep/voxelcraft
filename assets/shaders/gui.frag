#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main() {
    float mask = texture(uTexture, vTexCoord).r;
    FragColor = vec4(uColor.rgb, mask * uColor.a);
}
