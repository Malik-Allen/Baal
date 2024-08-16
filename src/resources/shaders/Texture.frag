#version 450

#define MAX_LIGHTS 8

struct DirectionalLight
{
	uint color;
	vec3 direction;
    float intensity;
};

struct PointLight {
    uint color;
    float intensity;
    float attenuation;
    vec3 position;
};

layout(binding = 2) uniform sampler2D texSampler;

layout(binding = 3) buffer direcLighting {
    DirectionalLight directionalLight;
};

layout(binding = 4) buffer pointLighting {
    PointLight pointLights[MAX_LIGHTS];
};

struct Material
{
	vec4 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

layout(push_constant) uniform constants {
    layout(offset = 64) Material material;
} fragConsts;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragTexCoord);
}