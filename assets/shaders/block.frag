#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in float vFaceIndex;
in vec3 WorldPos;

uniform float uOverlay;
uniform float uHighlight;
uniform vec3 uCameraPos;

const vec3 TOP_COLOR = vec3(0.2, 0.7, 0.1);
const vec3 SIDE_COLOR = vec3(0.5, 0.3, 0.1);
const vec3 CRACK_COLOR = vec3(0.0, 0.0, 0.0);
const vec3 HIGHLIGHT_COLOR = vec3(1.0, 1.0, 1.0);
const vec3 FOG_COLOR = vec3(1.0, 1.0, 1.0);

const float FOG_MIN_DIST = 5.0;
const float FOG_MAX_DIST = 50.0;
const float FOG_MAX_FACTOR = 0.5;

void main() {
    vec3 baseColor = (vFaceIndex > 3.5) ? TOP_COLOR : SIDE_COLOR;

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
        color = mix(color, HIGHLIGHT_COLOR, 0.5);
    }

    if (uOverlay > 0.0) {
        float crackAlpha = uOverlay * 0.7;
        color = mix(color, CRACK_COLOR, crackAlpha);
    }

    FragColor = vec4(color, 1.0);
}
