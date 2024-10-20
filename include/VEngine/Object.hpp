///
/// @file Object.hpp
/// @brief This file contains the Object class
/// @namespace ven
///

#pragma once

#include "VEngine/SwapChain.hpp"
#include "VEngine/Texture.hpp"
#include "VEngine/Model.hpp"
#include "VEngine/Transform3D.hpp"

namespace ven {

    static constexpr uint16_t MAX_OBJECTS = 1000;

    struct ObjectBufferData {
        glm::mat4 modelMatrix{1.F};
        glm::mat4 normalMatrix{1.F};
    };

    ///
    /// @class Object
    /// @brief Class for object
    /// @namespace ven
    ///
    class Object {

        public:

            using Map = std::unordered_map<unsigned int, Object>;

            explicit Object(const unsigned int objId) : m_objId{objId} {}

            ~Object() = default;

            Object(const Object &) = delete;
            Object &operator=(const Object &) = delete;
            Object(Object &&) = default;
            Object &operator=(Object &&) = default;

            [[nodiscard]] unsigned int getId() const { return m_objId; }
            [[nodiscard]] std::string getName() const { return m_name; }
            [[nodiscard]] std::shared_ptr<Model> getModel() const { return m_model; }
            [[nodiscard]] std::shared_ptr<Texture> getDiffuseMap() const { return m_diffuseMap; }
            [[nodiscard]] VkDescriptorBufferInfo getBufferInfo(const int frameIndex) const { return m_bufferInfo.at(frameIndex); }
            void setModel(const std::shared_ptr<Model> &model) { m_model = model; }
            void setDiffuseMap(const std::shared_ptr<Texture> &diffuseMap) { m_diffuseMap = diffuseMap; }
            void setName(const std::string &name) { m_name = name; }
            void setBufferInfo(const int frameIndex, const VkDescriptorBufferInfo& info) {
                m_bufferInfo[frameIndex] = info;
            }

            Transform3D transform{};

        private:

            unsigned int m_objId;
            std::string m_name;
            std::shared_ptr<Model> m_model = nullptr;
            std::shared_ptr<Texture> m_diffuseMap = nullptr;
            std::unordered_map<int, VkDescriptorBufferInfo> m_bufferInfo;

    }; // class Object

} // namespace ven
