#version 450

layout(binding = 0) uniform CameraMatrices {
    mat4 model;
    mat4 view;
    mat4 proj;
} camera;

struct PointLight {
    vec3 position;
    uint color;
};

layout(binding = 1) uniform PointLights {
    PointLight lights[4];
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

    float r = float((lights[1].color >> 0) & 0xFF) / 255.0;
    float g = float((lights[1].color >> 8) & 0xFF) / 255.0;
    float b = float((lights[1].color >> 16) & 0xFF) / 255.0;
    float a = float((lights[1].color >> 24) & 0xFF) / 255.0;

    fragColor = vec3(r, g, b);
    // fragColor = vec3(0.5, 0.5, 1.0);
    fragTexCoord = inTexCoords;
}