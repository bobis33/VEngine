#include <numeric>

#include "VEngine/SceneManager.hpp"

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
            MAX_GAME_OBJECTS,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            alignment);
        uboBuffer->map();
    }
    m_textureDefault = Texture::createTextureFromFile(device, "assets/textures/missing.png");
}

ven::Object& ven::SceneManager::createObject()
{
    assert(m_currentId < MAX_GAME_OBJECTS && "Max game object count exceeded!");
    auto object = Object{m_currentId++};
    auto objId = object.getId();
    object.setDiffuseMap(m_textureDefault);
    m_objects.emplace(objId, std::move(object));
    return m_objects.at(objId);
}

void ven::SceneManager::updateBuffer(unsigned long frameIndex)
{
    for (auto&[id, object] : m_objects) {
        const ObjectBufferData data{
            .modelMatrix = object.transform.mat4(),
            .normalMatrix = object.transform.normalMatrix()
        };
        m_uboBuffers.at(frameIndex)->writeToIndex(&data, id);

        object.setBufferInfo(static_cast<int>(frameIndex), m_uboBuffers.at(frameIndex)->descriptorInfoForIndex(id));
    }
    m_uboBuffers.at(frameIndex)->flush();
}
