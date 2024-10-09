///
/// @file RenderSystem.hpp
/// @brief This file contains the RenderSystem class
/// @namespace ven
///

#pragma once

#include "VEngine/FrameInfo.hpp"
#include "VEngine/Abstraction/ARenderSystemBase.hpp"

namespace ven {

    struct ObjectPushConstantData {
        glm::mat4 modelMatrix{};
        glm::mat4 normalMatrix{};
    };

    ///
    /// @class RenderSystem
    /// @brief Class for render system
    /// @namespace ven
    ///
    class RenderSystem : public ARenderSystemBase {

        public:

            explicit RenderSystem(Device& device, const VkRenderPass renderPass, const VkDescriptorSetLayout globalSetLayout) : ARenderSystemBase(device) {
                createPipelineLayout(globalSetLayout, sizeof(ObjectPushConstantData));
                createPipeline(renderPass, std::string(SHADERS_BIN_PATH) + "shader_vert.spv", std::string(SHADERS_BIN_PATH) + "shader_frag.spv", false);
            }

            RenderSystem(const RenderSystem&) = delete;
            RenderSystem& operator=(const RenderSystem&) = delete;

            void renderObjects(const FrameInfo &frameInfo) const;

    }; // class RenderSystem

} // namespace ven
