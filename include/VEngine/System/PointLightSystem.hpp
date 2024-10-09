///
/// @file PointLightSystem.hpp
/// @brief This file contains the PointLightSystem class
/// @namespace ven
///

#pragma once

#include "VEngine/Abstraction/ARenderSystemBase.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    struct LightPushConstantData {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius;
    };

    ///
    /// @class PointLightSystem
    /// @brief Class for point light system
    /// @namespace ven
    ///
    class PointLightSystem : public ARenderSystemBase {

        public:

            explicit PointLightSystem(Device& device, const VkRenderPass renderPass, const VkDescriptorSetLayout globalSetLayout) : ARenderSystemBase(device) {
                createPipelineLayout(globalSetLayout, sizeof(LightPushConstantData));
                createPipeline(renderPass, std::string(SHADERS_BIN_PATH) + "point_light_vert.spv", std::string(SHADERS_BIN_PATH) + "point_light_frag.spv", true);
            }

            void render(const FrameInfo &frameInfo) const;
            static void update(const FrameInfo &frameInfo, GlobalUbo &ubo);

    }; // class PointLightSystem

} // namespace ven
