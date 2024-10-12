///
/// @file Shaders.hpp
/// @brief This file contains the Shader class
/// @namespace ven
///

#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include "VEngine/Device.hpp"

namespace ven {

    static constexpr std::string_view SHADERS_BIN_PATH = "shaders/bin/";

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
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

    ///
    /// @class Shaders
    /// @brief Class for shaders
    /// @namespace ven
    ///
    class Shaders {

        public:

            Shaders(Device &device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo) : m_device{device} { createGraphicsPipeline(vertFilepath, fragFilepath, configInfo); };
            ~Shaders();

            Shaders(const Shaders&) = delete;
            Shaders& operator=(const Shaders&) = delete;

            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
            void bind(const VkCommandBuffer commandBuffer) const { vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline); }

        private:

            static std::vector<char> readFile(const std::string &filename);
            void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const;

            Device& m_device;
            VkPipeline m_graphicsPipeline{nullptr};
            VkShaderModule m_vertShaderModule{nullptr};
            VkShaderModule m_fragShaderModule{nullptr};

    }; // class Shaders

} // namespace ven
