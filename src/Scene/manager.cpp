#include <numeric>
#include <ranges>

#include "VEngine/Scene/Manager.hpp"

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
    m_textureDefault = Texture::createTextureFromFile(device, "assets/textures/default.png");
}

ven::Object& ven::SceneManager::createObject()
{
    assert(m_currentObjId < MAX_OBJECTS && "Max object count exceeded!");
    Object object(m_currentObjId++);
    const unsigned int objId = object.getId();
    object.setDiffuseMap(m_textureDefault);
    m_objects.emplace(objId, std::move(object));
    return m_objects.at(objId);
}

ven::Object& ven::SceneManager::duplicateObject(const unsigned int objectId)
{
    const Object &cpyObj = m_objects.at(objectId);
    Object &object = createObject();
    object.setName(cpyObj.getName());
    object.setModel(cpyObj.getModel());
    object.transform = cpyObj.transform;
    object.setDiffuseMap(cpyObj.getDiffuseMap());
    return object;
}

ven::Light& ven::SceneManager::createLight(const float radius, const glm::vec4 color)
{
    assert(m_currentLightId < MAX_LIGHTS && "Max light count exceeded!");
    Light light(m_currentLightId++);
    const unsigned int lightId = light.getId();
    light.color = color;
    light.transform.scale.x = radius;
    m_lights.emplace(lightId, std::move(light));
    return m_lights.at(lightId);
}

ven::Light& ven::SceneManager::duplicateLight(const unsigned int lightId)
{
    const Light &cpyLight = m_lights.at(lightId);
    Light& light = createLight(cpyLight.transform.scale.x, cpyLight.color);
    light.transform = cpyLight.transform;
    return light;
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

void ven::SceneManager::destroyEntity(std::vector<unsigned int> *objectsIds, std::vector<unsigned int> *lightsIds)
{
    for (const unsigned int objectId : *objectsIds) {
        m_objects.erase(objectId);
    }
    for (const unsigned int lightId : *lightsIds) {
        m_lights.erase(lightId);
    }
    objectsIds->clear();
    lightsIds->clear();
    m_destroyState = false;
}
