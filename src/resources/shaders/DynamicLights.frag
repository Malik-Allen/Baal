#version 450

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D texSampler;

layout(push_constant) uniform constants {
    layout(offset = 64) Material material;
} fragConstants;

void main() {
	vec3 color;
	color.x = fragConstants.material.diffuse.x;
	color.y = fragConstants.material.diffuse.y;
	color.z = fragConstants.material.diffuse.z;
    outColor = vec4(color, 1.0);
}