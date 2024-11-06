///
/// @file Manager.hpp
/// @brief This file contains the SceneManager class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/FrameInfo.hpp"
#include "VEngine/Gfx/SwapChain.hpp"

namespace ven {

    ///
    /// @class SceneManager
    /// @brief Class for object manager
    /// @namespace ven
    ///
    class SceneManager {

        public:

            explicit SceneManager(Device &device);

            SceneManager(const SceneManager &) = delete;
            SceneManager &operator=(const SceneManager &) = delete;
            SceneManager(SceneManager &&) = delete;
            SceneManager &operator=(SceneManager &&) = delete;

            void addObject(const std::unique_ptr<Object> &object) { m_objects.insert({object->getId(), std::move(*object)}); }
            void addLight(const std::unique_ptr<Light> &light) { m_lights.insert({light->getId(), std::move(*light)}); }

            void destroyObject(const unsigned int objectId) { m_objects.erase(objectId); }
            void destroyLight(const unsigned int lightId) { m_lights.erase(lightId); }
            void destroyEntity(std::vector<unsigned int> *objectsIds, std::vector<unsigned int> *lightsIds);

            void updateBuffer(GlobalUbo &ubo, unsigned long frameIndex, float frameTime);

            [[nodiscard]] VkDescriptorBufferInfo getBufferInfoForObject(const int frameIndex, const unsigned int objectId) const { return m_uboBuffers.at(static_cast<long unsigned int>(frameIndex))->descriptorInfoForIndex(objectId); }
            Object::Map& getObjects() { return m_objects; }
            Light::Map& getLights() { return m_lights; }
            std::vector<std::unique_ptr<Buffer>> &getUboBuffers() { return m_uboBuffers; }
            std::shared_ptr<Texture> getTextureDefault() { return m_textureDefault; }
            [[nodiscard]] bool getDestroyState() const { return m_destroyState; }

            void setDestroyState(const bool state) { m_destroyState = state; }

        private:

            unsigned int m_currentObjId{0};
            unsigned int m_currentLightId{0};
            std::shared_ptr<Texture> m_textureDefault;
            Object::Map m_objects;
            Light::Map m_lights;
            std::vector<std::unique_ptr<Buffer>> m_uboBuffers{MAX_FRAMES_IN_FLIGHT};
            bool m_destroyState{false};

    }; // class SceneManager

} // namespace ven
