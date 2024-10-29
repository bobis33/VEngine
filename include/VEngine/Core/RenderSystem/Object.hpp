///
/// @file Object.hpp
/// @brief This file contains the ObjectRenderSystem class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/RenderSystem/ABase.hpp"

namespace ven {

    struct ObjectPushConstantData {
        glm::mat4 modelMatrix{};
        glm::mat4 normalMatrix{};
    };

    ///
    /// @class ObjectRenderSystem
    /// @brief Class for object render system
    /// @namespace ven
    ///
    class ObjectRenderSystem final : public ARenderSystemBase {

        public:

            explicit ObjectRenderSystem(Device& device, const VkRenderPass renderPass, const VkDescriptorSetLayout globalSetLayout) : ARenderSystemBase(device) {
                createPipelineLayout(globalSetLayout, sizeof(ObjectPushConstantData));
                createPipeline(renderPass, std::string(SHADERS_BIN_PATH) + "vertex_shader.spv", std::string(SHADERS_BIN_PATH) + "fragment_shader.spv", false);
            }

            ObjectRenderSystem(const ObjectRenderSystem&) = delete;
            ObjectRenderSystem& operator=(const ObjectRenderSystem&) = delete;

            void render(const FrameInfo &frameInfo) const override;

    }; // class ObjectRenderSystem

} // namespace ven
