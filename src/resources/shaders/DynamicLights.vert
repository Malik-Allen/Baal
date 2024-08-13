#version 450

layout(binding = 0) uniform CameraMatrices {
    mat4 model;
    mat4 view;
    mat4 proj;
} camera;

struct PointLight {
    uint color;
    vec3 position;
};

struct DirectionalLight
{
	uint color;
	float intensity;
	vec3 direction;
};

struct TestLight
{
	vec4 pos;
	vec4 color;
};

layout(binding = 1) uniform TestLights {
    TestLight lights;
};

layout(binding = 3) buffer direcLighting {
    DirectionalLight directionalLight;
};

layout(push_constant) uniform constants {
    mat4 model;
} mesh;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = camera.proj * camera.view * mesh.model * vec4(inPos, 1.0);

    DirectionalLight light = directionalLight;
    float r = float((light.color >> 0) & 0xFF) / 255.0;
    float g = float((light.color >> 8) & 0xFF) / 255.0;
    float b = float((light.color >> 16) & 0xFF) / 255.0;
    float a = float((light.color >> 24) & 0xFF) / 255.0;

    // fragColor = vec3(light.color.x, light.color.y, light.color.z);
    fragColor = vec3(r, g, b);
    fragTexCoord = inTexCoords;
}