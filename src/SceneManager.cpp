#include <numeric>
#include <ranges>

#include "VEngine/SceneManager.hpp"

#include <VEngine/FrameInfo.hpp>

ven::SceneManager::SceneManager(Device& device)
{
    // including nonCoherentAtomSize allows us to flush a specific index at once
    unsigned long alignment = std::lcm(
        device.getProperties().limits.nonCoherentAtomSize,
        device.getProperties().limits.minUniformBufferOffsetAlignment
        );
    for (auto & uboBuffer : m_uboBuffers) {
        uboBuffer = std::make_unique<Buffer>(
            device,
            sizeof(ObjectBufferData),
            MAX_OBJECTS,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            alignment);
        uboBuffer->map();
    }
    m_textureDefault = Texture::createTextureFromFile(device, "assets/textures/missing.png");
}

ven::Object& ven::SceneManager::createObject()
{
    assert(m_currentObjId < MAX_OBJECTS && "Max object count exceeded!");
    auto object = Object{m_currentObjId++};
    auto objId = object.getId();
    object.setDiffuseMap(m_textureDefault);
    m_objects.emplace(objId, std::move(object));
    return m_objects.at(objId);
}

ven::Light& ven::SceneManager::createLight(const float radius, const glm::vec4 color)
{
    assert(m_currentLightId < MAX_LIGHTS && "Max light count exceeded!");
    auto light = Light(m_currentLightId++);
    auto lightId = light.getId();
    light.color = color;
    light.transform.scale.x = radius;
    m_lights.emplace(lightId, std::move(light));
    return m_lights.at(lightId);
}

void ven::SceneManager::updateBuffer(const unsigned long frameIndex, GlobalUbo &ubo, const float frameTime)
{
    uint16_t lightIndex = 0;
    const glm::mat4 rotateLight = rotate(glm::mat4(1.F), frameTime, {0.F, -1.F, 0.F});

    for (auto& [id, object] : m_objects) {
        const ObjectBufferData data{
            .modelMatrix = object.transform.mat4(),
            .normalMatrix = object.transform.normalMatrix()
        };
        m_uboBuffers.at(frameIndex)->writeToIndex(&data, id);
        object.setBufferInfo(static_cast<int>(frameIndex), m_uboBuffers.at(frameIndex)->descriptorInfoForIndex(id));
    }

    for (Light &light : m_lights | std::views::values) {
        light.transform.translation = glm::vec3(rotateLight * glm::vec4(light.transform.translation, 1.F));
        ubo.pointLights.at(lightIndex).position = glm::vec4(light.transform.translation, 1.F);
        ubo.pointLights.at(lightIndex).color = light.color;
        lightIndex++;
    }
    ubo.numLights = lightIndex;
}
