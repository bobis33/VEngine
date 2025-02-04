#include <numeric>
#include <ranges>

#include "VEngine/Factories/Texture.hpp"
#include "VEngine/Scene/Manager.hpp"
#include "VEngine/Utils/Logger.hpp"

ven::SceneManager::SceneManager(const Device& device)
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
    Logger::logExecutionTime("Creating default texture", [&] {
        m_textureDefault = TextureFactory::create(device, "assets/textures/owned/default.png");
    });
}

void ven::SceneManager::updateBuffer(GlobalUbo &ubo, const unsigned long frameIndex, const float frameTime)
{
    uint8_t lightIndex = 0;
    const glm::mat4 rotateLight = rotate(glm::mat4(1.F), frameTime, {0.F, -1.F, 0.F});

    for (Object& object : m_objects | std::views::values) {
        const ObjectBufferData data{
            .modelMatrix = object.transform.transformMatrix(),
            .normalMatrix = object.transform.normalMatrix()
        };
        m_uboBuffers.at(frameIndex)->writeToIndex(&data, object.getId());
        object.setBufferInfo(static_cast<int>(frameIndex), m_uboBuffers.at(frameIndex)->descriptorInfoForIndex(object.getId()));
    }

    for (Light &light : m_lights | std::views::values) {
        auto&[position, color, shininess, padding] = ubo.pointLights.at(lightIndex);
        light.transform.translation = glm::vec3(rotateLight * glm::vec4(light.transform.translation, light.transform.scale.x));
        position = glm::vec4(light.transform.translation, light.transform.scale.x);
        color = light.color;
        shininess = light.getShininess();
        lightIndex++;
    }
    ubo.numLights = lightIndex;
}

void ven::SceneManager::destroyEntity(std::vector<unsigned int>& objectsIds, std::vector<unsigned int>& lightsIds)
{
    for (const unsigned int objectId : objectsIds) {
        m_objects.erase(objectId);
    }
    for (const unsigned int lightId : lightsIds) {
        m_lights.erase(lightId);
    }
    objectsIds.clear();
    lightsIds.clear();
    m_destroyState = false;
}
