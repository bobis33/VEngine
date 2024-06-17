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

namespace ven {

    struct PipelineConfigInfo {
        VkViewport viewport{};
        VkRect2D scissor{};
        VkPipelineViewportStateCreateInfo viewportInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Shaders {

        public:

            Shaders(Device &device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
            ~Shaders();

            Shaders(const Shaders&) = delete;
            void operator=(const Shaders&) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:

            static std::vector<char> readFile(const std::string &filename);

            void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            Device& lveDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

    }; // class Shaders

} // namespace ven
