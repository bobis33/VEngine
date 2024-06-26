/*
** @file Shaders.hpp
** @brief This file contains the Shader class
** @namespace ven
*/

#pragma once

#include <vector>
#include <array>
#include <string>
#include <cassert>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "VEngine/Device.hpp"
#include "VEngine/Model.hpp"

namespace ven {

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Shaders {

        public:

            Shaders(Device &device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo) : m_device{device} { createGraphicsPipeline(vertFilepath, fragFilepath, configInfo); };
            ~Shaders();

            Shaders(const Shaders&) = delete;
            Shaders& operator=(const Shaders&) = delete;

            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
            void bind(VkCommandBuffer commandBuffer) { vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline); };

        private:

            static std::vector<char> readFile(const std::string &filename);
            void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            Device& m_device;
            VkPipeline m_graphicsPipeline;
            VkShaderModule m_vertShaderModule;
            VkShaderModule m_fragShaderModule;

    }; // class Shaders

} // namespace ven
