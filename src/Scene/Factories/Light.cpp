#include "VEngine/Scene/Factories/Light.hpp"

unsigned int ven::LightFactory::m_currentLightId = 0;

std::unique_ptr<ven::Light> ven::LightFactory::createLight(const Transform3D &transform, const glm::vec4 color)
{
    assert(m_currentLightId < MAX_LIGHTS && "Max light count exceeded!");
    auto light = std::make_unique<Light>(++m_currentLightId);
    light->color = color;
    light->transform = transform;
    return light;
}

std::unique_ptr<ven::Light> ven::LightFactory::duplicateLight(const Light &cpyLight)
{
    return createLight(cpyLight.transform, cpyLight.color);
}
