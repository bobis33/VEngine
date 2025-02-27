#version 450

layout(set = 0, binding = 1) uniform sampler2D textures[1];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragAmbientColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) flat in uint fragTextureIndex;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 texColor = texture(textures[fragTextureIndex], fragTexCoord);
    vec3 finalColor = mix(texColor.rgb, texColor.rgb * fragAmbientColor, 1.0); // 1.0 is the ambient strength, TODO: make a vec4 and use the alpha channel for the strength
    outColor = vec4(finalColor, texColor.a);
}
