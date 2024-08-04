#version 450

layout(binding = 0) uniform CameraMatrices {
    mat4 model;
    mat4 view;
    mat4 proj;
} camera;

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
    fragColor = vec3(1.0, 1.0, 1.0);
    fragTexCoord = inTexCoords;
}