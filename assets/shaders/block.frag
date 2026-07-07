#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 uColor;

void main() {
    vec3 dist = min(fract(FragPos), 1.0 - fract(FragPos));

    // Face normal direction has dist ≈ 0 — ignore it so only the
    // two varying axes contribute to the edge line.
    vec3 masked = mix(dist, vec3(1.0), step(dist, vec3(0.001)));

    float edgeDist = min(min(masked.x, masked.y), masked.z);
    float edgeWidth = 0.04;
    float t = 1.0 - smoothstep(0.0, edgeWidth, edgeDist);

    vec3 edgeColor = vec3(0.05);
    vec3 color = mix(uColor, edgeColor, t);

    FragColor = vec4(color, 1.0);
}
