///
/// @file SceneManager.hpp
/// @brief This file contains the SceneManager class
/// @namespace ven
///

#pragma once

#include "VEngine/FrameInfo.hpp"

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

            Object& createObject();
            Object& duplicateObject(unsigned int objectId);
            Light& createLight(float radius = DEFAULT_LIGHT_RADIUS, glm::vec4 color = DEFAULT_LIGHT_COLOR);
            Light& duplicateLight(unsigned int lightId);

            void destroyObject(const unsigned int objectId) { m_objects.erase(objectId); }
            void destroyLight(const unsigned int lightId) { m_lights.erase(lightId); }
            void destroyEntity(std::vector<unsigned int> *objectsIds, std::vector<unsigned int> *lightsIds);

            void updateBuffer(GlobalUbo &ubo, unsigned long frameIndex, float frameTime);

            VkDescriptorBufferInfo getBufferInfoForObject(const int frameIndex, const unsigned int objectId) const { return m_uboBuffers.at(static_cast<long unsigned int>(frameIndex))->descriptorInfoForIndex(objectId); }
            Object::Map& getObjects() { return m_objects; }
            Light::Map& getLights() { return m_lights; }
            std::vector<std::unique_ptr<Buffer>> &getUboBuffers() { return m_uboBuffers; }
            bool getDestroyState() const { return m_destroyState; }

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
