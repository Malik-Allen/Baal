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
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 inNorm;
layout(location = 4) in vec3 eyePos;

layout(location = 0) out vec4 outColor;

vec4 getColor(uint color)
{
    float r = float((color >> 0) & 0xFF) / 255.0;
    float g = float((color >> 8) & 0xFF) / 255.0;
    float b = float((color >> 16) & 0xFF) / 255.0;
    float a = float((color >> 24) & 0xFF) / 255.0;
    return vec4(r, g, b, a);
}

void main() {
    vec3 lightColor = vec3(getColor(directionalLight.color));

    // Ambient
    vec3 ambient = vec3(fragConsts.material.ambient) * lightColor;

    // Diffuse
    vec3 lightDirec = normalize(-directionalLight.direction);
    vec3 norm = normalize(inNorm);
    float diff = max(dot(norm, lightDirec), 0.0);
    vec3 diffuse = lightColor * (diff * fragConsts.material.diffuse);

    // Specular
    vec3 viewDir = normalize(eyePos - fragPos);
    vec3 reflectDir = reflect(-lightDirec, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragConsts.material.shininess);
    vec3 specular = lightColor * (spec * fragConsts.material.specular); 

    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
}