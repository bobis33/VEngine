///
/// @file PointLightSystem.hpp
/// @brief This file contains the PointLightSystem class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    ///
    /// @class PointLightSystem
    /// @brief Class for point light system
    /// @namespace ven
    ///
    class PointLightSystem {

        public:

            explicit PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~PointLightSystem() { vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr); }

            PointLightSystem(const PointLightSystem&) = delete;
            PointLightSystem& operator=(const PointLightSystem&) = delete;

            static void update(const FrameInfo &frameInfo, GlobalUbo &ubo);
            void render(const FrameInfo &frameInfo) const;

        private:

            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);

            Device &m_device;

            std::unique_ptr<Shaders> m_shaders;
            VkPipelineLayout m_pipelineLayout{nullptr};

    }; // class PointLightSystem

} // namespace ven
