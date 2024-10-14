#include "VEngine/Light.hpp"

ven::Light ven::Light::createLight(const float radius, const glm::vec4 color)
{
    static unsigned int objId = 0;
    Light light(objId++);

    light.color = color;
    light.transform3D.scale.x = radius;

    return light;
}