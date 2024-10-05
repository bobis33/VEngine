///
/// @file RenderSystem.hpp
/// @brief This file contains the RenderSystem class
/// @namespace ven
///

#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.F};
        glm::mat4 normalMatrix{1.F};
    };

    ///
    /// @class RenderSystem
    /// @brief Class for render system
    /// @namespace ven
    ///
    class RenderSystem {

        public:

            explicit RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~RenderSystem() { vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr); }

            RenderSystem(const RenderSystem&) = delete;
            RenderSystem& operator=(const RenderSystem&) = delete;

            void renderObjects(const FrameInfo &frameInfo) const;

        private:

            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);

            Device &m_device;
            std::unique_ptr<Shaders> m_shaders;
            VkPipelineLayout m_pipelineLayout{nullptr};

    }; // class RenderSystem

} // namespace ven
