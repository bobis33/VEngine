///
/// @file PointLight.hpp
/// @brief This file contains the PointLightRenderSystem class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/RenderSystem/ABase.hpp"

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
    class PointLightRenderSystem final : public ARenderSystemBase {

        public:

            explicit PointLightRenderSystem(Device& device, const VkRenderPass renderPass, const VkDescriptorSetLayout globalSetLayout) : ARenderSystemBase(device) {
                createPipelineLayout(globalSetLayout, sizeof(LightPushConstantData));
                createPipeline(renderPass, std::string(SHADERS_BIN_PATH) + "vertex_point_light.spv", std::string(SHADERS_BIN_PATH) + "fragment_point_light.spv", true);
            }

            PointLightRenderSystem(const PointLightRenderSystem&) = delete;
            PointLightRenderSystem& operator=(const PointLightRenderSystem&) = delete;
            PointLightRenderSystem(PointLightRenderSystem&&) = delete;
            PointLightRenderSystem& operator=(PointLightRenderSystem&&) = delete;

            void render(const FrameInfo &frameInfo) const override;

    }; // class PointLightRenderSystem

} // namespace ven
