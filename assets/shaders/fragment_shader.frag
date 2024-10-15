#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout(location = 0) out vec4 outColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec3 specularLight = vec3(0.0);
  vec3 surfaceNormal = normalize(gl_FrontFacing ? fragNormalWorld : -fragNormalWorld);
  vec3 diffuseLight = ubo.ambientLightColor.rgb * ubo.ambientLightColor.a;

  vec3 cameraPosWorld = ubo.invView[3].xyz;
  vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

  for (int i = 0; i < ubo.numLights; i++) {
    PointLight light = ubo.pointLights[i];
    vec3 directionToLight = light.position.xyz - fragPosWorld;
    float distanceSquared = dot(directionToLight, directionToLight);
    float attenuation = 1.0 / distanceSquared; // distance squared
    directionToLight = normalize(directionToLight);

    float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
    vec3 intensity = light.color.rgb * light.color.a * attenuation;
    vec3 reflectionDirection = reflect(-directionToLight, surfaceNormal);
    float cosAngReflection = max(dot(viewDirection, reflectionDirection), 0);

    // diffuse lighting
    diffuseLight += intensity * cosAngIncidence;
    // specular lighting
    float specular = pow(cosAngReflection, 512);
    specularLight += intensity * specular * step(0.0, cosAngIncidence) * step(0.0, cosAngReflection);
  }

  outColor = vec4(fragColor * (diffuseLight + specularLight), 1.0);
}
