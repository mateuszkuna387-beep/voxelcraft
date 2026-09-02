#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in float vFaceIndex;
in float vBlockId;
in vec3 WorldPos;

uniform float uOverlay;
uniform float uHighlight;
uniform vec3 uCameraPos;

const vec3 CRACK_COLOR = vec3(0.0, 0.0, 0.0);
const vec3 HIGHLIGHT_COLOR = vec3(1.0, 1.0, 1.0);
const vec3 FOG_COLOR = vec3(1.0, 1.0, 1.0);

const float FOG_MIN_DIST = 5.0;
const float FOG_MAX_DIST = 50.0;
const float FOG_MAX_FACTOR = 0.5;

vec3 getBlockColor(int id, float face) {
    if (id == 1) return vec3(0.50, 0.50, 0.52);
    if (id == 2) return vec3(0.55, 0.33, 0.14);
    if (id == 3) {
        if (face > 3.5 && face < 4.5) return vec3(0.25, 0.62, 0.18);
        if (face > 4.5) return vec3(0.55, 0.33, 0.14);
        float topStrip = step(0.85, fract(FragPos.y));
        vec3 sideBase = vec3(0.38, 0.28, 0.10);
        vec3 topGreen = vec3(0.25, 0.62, 0.18);
        return mix(sideBase, topGreen, topStrip);
    }
    if (id == 4) {
        if (face > 3.5 && face < 4.5) return vec3(0.65, 0.45, 0.20);
        if (face > 4.5) return vec3(0.65, 0.45, 0.20);
        float ring = step(0.33, fract(FragPos.y)) * (1.0 - step(0.66, fract(FragPos.y)));
        vec3 bark = vec3(0.40, 0.26, 0.13);
        vec3 inner = vec3(0.55, 0.35, 0.15);
        return mix(bark, inner, ring * 0.3);
    }
    if (id == 5) return vec3(0.18, 0.55, 0.18);
    if (id == 6) return vec3(0.76, 0.70, 0.50);
    if (id == 7) return vec3(0.20, 0.40, 0.80);
    if (id == 8) return vec3(0.30, 0.30, 0.30);
    return vec3(0.50, 0.30, 0.10);
}

void main() {
    int blockId = int(vBlockId + 0.5);
    vec3 baseColor = getBlockColor(blockId, vFaceIndex);

    vec3 dist = min(fract(FragPos), 1.0 - fract(FragPos));
    vec3 masked = mix(dist, vec3(1.0), step(dist, vec3(0.001)));

    float edgeDist = min(min(masked.x, masked.y), masked.z);
    float edgeWidth = 0.04;
    float t = 1.0 - smoothstep(0.0, edgeWidth, edgeDist);

    vec3 edgeColor = vec3(0.05);
    vec3 color = mix(baseColor, edgeColor, t);

    float distToCamera = length(WorldPos - uCameraPos);
    float fogFactor = smoothstep(FOG_MIN_DIST, FOG_MAX_DIST, distToCamera);
    fogFactor *= FOG_MAX_FACTOR;
    color = mix(color, FOG_COLOR, fogFactor);

    if (uHighlight > 0.0) {
        color = mix(color, HIGHLIGHT_COLOR, 0.25);
    }

    if (uOverlay > 0.0) {
        float crackAlpha = uOverlay * 0.7;
        color = mix(color, CRACK_COLOR, crackAlpha);
    }

    FragColor = vec4(color, 1.0);
}
