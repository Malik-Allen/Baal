#version 450

layout(binding = 0) buffer CameraMatrix {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 pos;
} camera;

layout(push_constant) uniform constants {
    mat4 model;
} vertConsts;

struct TestLight
{
	vec4 pos;
	vec4 color;
};

layout(binding = 1) uniform TestLights {
    TestLight lights;
};

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPos;
layout(location = 3) out vec3 outNorm;
layout(location = 4) out vec3 eyePos;

void main() {
    gl_Position = camera.proj * camera.view * vertConsts.model * vec4(inPos, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoords;
    fragPos = vec3(vertConsts.model * vec4(inPos, 1.0));
    outNorm = mat3(transpose(inverse(vertConsts.model))) * inNorm;  // In order to apply any tranformation applied to the model, to its normals as well
    eyePos = vec3(camera.pos);
}