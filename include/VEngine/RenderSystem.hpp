/*
** @file RenderSystem.hpp
** @brief This file contains the RenderSystem class
** @namespace ven
*/

#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/Model.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/Camera.hpp"

namespace ven {

    struct SimplePushConstantData {
        glm::mat4 transform{1.F};
        alignas(16) glm::vec3 color;
    };

    class RenderSystem {

    public:

        explicit RenderSystem(Device& device, VkRenderPass renderPass);
        ~RenderSystem();

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void renderObjects(VkCommandBuffer commandBuffer, std::vector<ven::Object>& objects, const Camera& camera);

    private:

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device &m_device;

        std::unique_ptr<Shaders> m_shaders;
        VkPipelineLayout m_pipelineLayout{nullptr};

    }; // class RenderSystem

} // namespace ven