#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in float vFaceIndex;

const vec3 TOP_COLOR = vec3(0.2, 0.7, 0.1);
const vec3 SIDE_COLOR = vec3(0.5, 0.3, 0.1);

void main() {
    vec3 baseColor = (vFaceIndex > 3.5) ? TOP_COLOR : SIDE_COLOR;

    vec3 dist = min(fract(FragPos), 1.0 - fract(FragPos));
    vec3 masked = mix(dist, vec3(1.0), step(dist, vec3(0.001)));

    float edgeDist = min(min(masked.x, masked.y), masked.z);
    float edgeWidth = 0.04;
    float t = 1.0 - smoothstep(0.0, edgeWidth, edgeDist);

    vec3 edgeColor = vec3(0.05);
    vec3 color = mix(baseColor, edgeColor, t);

    FragColor = vec4(color, 1.0);
}
