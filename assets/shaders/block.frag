#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D uTexture;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform float uAmbientStrength;

void main() {
    vec3 ambient = uAmbientStrength * uLightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    vec4 texColor = texture(uTexture, TexCoord);
    vec3 result = (ambient + diffuse) * texColor.rgb;

    FragColor = vec4(result, texColor.a);
}
