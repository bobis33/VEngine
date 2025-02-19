#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragUv;

layout(location = 0) out vec4 outColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
  float shininess;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D diffuseMap;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec4 texColor = texture(diffuseMap, fragUv); // Couleur et alpha de la texture
  vec3 color = texColor.rgb;
  float alpha = texColor.a; // Canal alpha

  // Si le pixel est transparent, on garde la transparence sans éclairage
  if (alpha < 0.01) {
    outColor = vec4(0.0, 0.0, 0.0, 0.0); // Transparent
    return;
  }

  vec3 specularLight = vec3(0.0);
  vec3 surfaceNormal = normalize(fragNormalWorld);
  vec3 diffuseLight = ubo.ambientLightColor.rgb * ubo.ambientLightColor.a;

  vec3 cameraPosWorld = ubo.invView[3].xyz;
  vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

  for (int i = 0; i < ubo.numLights; i++) {
    PointLight light = ubo.pointLights[i];
    vec3 directionToLight = light.position.xyz - fragPosWorld;
    float distanceSquared = dot(directionToLight, directionToLight);
    float attenuation = distanceSquared > 0.001 ? (light.position.w + 1.0) / distanceSquared : 0.0;
    directionToLight = normalize(directionToLight);

    float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
    vec3 intensity = light.color.rgb * light.color.a * attenuation;

    if (cosAngIncidence > 0) {
      vec3 halfVector = normalize(directionToLight + viewDirection);
      float cosAngHalf = max(dot(surfaceNormal, halfVector), 0);

      float specular = pow(cosAngHalf, light.shininess);

      diffuseLight += intensity * cosAngIncidence;
      specularLight += intensity * specular;
    }
  }

  // Combine diffuse, specular, and texture color
  vec3 finalColor = diffuseLight * color + specularLight;
  outColor = vec4(finalColor, alpha); // Conserver la transparence d'origine
}

