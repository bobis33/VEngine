/*
** @file RenderSystem.hpp
** @brief This file contains the RenderSystem class
** @namespace ven
*/

#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "VEngine/Camera.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.F};
        glm::mat4 normalMatrix{1.F};
    };

    class RenderSystem {

    public:

        explicit RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void renderObjects(FrameInfo &frameInfo, std::vector<ven::Object>& objects);

    private:

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        Device &m_device;

        std::unique_ptr<Shaders> m_shaders;
        VkPipelineLayout m_pipelineLayout{nullptr};

    }; // class RenderSystem

} // namespace ven
