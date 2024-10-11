#include "VEngine/Light.hpp"

ven::Light ven::Light::createLight(float radius, glm::vec4 color)
{
    static unsigned int objId = 0;
    Light light = Light(objId++);

    light.color = color;
    light.transform3D.scale.x = radius;

    return light;
}