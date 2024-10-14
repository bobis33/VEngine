///
/// @file PointLightRenderSystem.hpp
/// @brief This file contains the PointLightRenderSystem class
/// @namespace ven
///

#pragma once

#include "VEngine/System/ARenderSystemBase.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    struct LightPushConstantData {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius;
    };

    ///
    /// @class PointLightRenderSystem
    /// @brief Class for point light system
    /// @namespace ven
    ///
    class PointLightRenderSystem : public ARenderSystemBase {

        public:

            explicit PointLightRenderSystem(Device& device, const VkRenderPass renderPass, const VkDescriptorSetLayout globalSetLayout) : ARenderSystemBase(device) {
                createPipelineLayout(globalSetLayout, sizeof(LightPushConstantData));
                createPipeline(renderPass, std::string(SHADERS_BIN_PATH) + "point_light_vert.spv", std::string(SHADERS_BIN_PATH) + "point_light_frag.spv", true);
            }

            PointLightRenderSystem(const PointLightRenderSystem&) = delete;
            PointLightRenderSystem& operator=(const PointLightRenderSystem&) = delete;

            void render(const FrameInfo &frameInfo) const override;

            static void update(const FrameInfo &frameInfo, GlobalUbo &ubo);

    }; // class PointLightRenderSystem

} // namespace ven
