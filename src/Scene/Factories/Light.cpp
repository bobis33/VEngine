#include "VEngine/Scene/Factories/Light.hpp"

unsigned int ven::LightFactory::m_currentLightId = 0;

std::unique_ptr<ven::Light> ven::LightFactory::createLight(const float radius, const glm::vec4 color)
{
    assert(m_currentLightId < MAX_LIGHTS && "Max light count exceeded!");
    auto light = std::make_unique<Light>(++m_currentLightId);
    light->color = color;
    light->transform.scale.x = radius;
    return light;
}

std::unique_ptr<ven::Light> ven::LightFactory::duplicateLight(const Light &cpyLight)
{
    std::unique_ptr<Light> light = createLight(cpyLight.transform.scale.x, cpyLight.color);
    light->transform = cpyLight.transform;
    return light;
}
