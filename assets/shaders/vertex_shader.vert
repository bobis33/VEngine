#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 ambientColor;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint textureIndex;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragAmbientColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) flat out uint fragTextureIndex;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragAmbientColor = ubo.ambientColor;
    fragTexCoord = inTexCoord;
    fragTextureIndex = textureIndex;
}