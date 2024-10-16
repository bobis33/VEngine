///
/// @file SceneManager.hpp
/// @brief This file contains the SceneManager class
/// @namespace ven
///
///
///
#pragma once

#include "VEngine/Object.hpp"

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

        Object &createObject();

        VkDescriptorBufferInfo getBufferInfoForObject(const int frameIndex, const unsigned int objectId) const { return m_uboBuffers.at(static_cast<long unsigned int>(frameIndex))->descriptorInfoForIndex(objectId); }
        Object::Map &getObjects() { return m_objects; }
        std::vector<std::unique_ptr<Buffer>> &getUboBuffers() { return m_uboBuffers; }

        void updateBuffer(unsigned long frameIndex);

    private:

        unsigned int m_currentId{0};
        std::shared_ptr<Texture> m_textureDefault;
        Object::Map m_objects;
        std::vector<std::unique_ptr<Buffer>> m_uboBuffers{MAX_FRAMES_IN_FLIGHT};

    }; // class SceneManager

} // namespace ven
