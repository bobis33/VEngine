///
/// @file Shaders.hpp
/// @brief This file contains the Shader class
/// @namespace ven
///

#pragma once

#include <vulkan/vulkan_core.h>

namespace ven {

    static constexpr std::string_view SHADERS_BIN_PATH = "build/shaders/";

    ///
    /// @class Shaders
    /// @brief Class for shaders
    /// @namespace ven
    ///
    class Shaders {

        public:

            explicit Shaders(const VkDevice& device) : m_device{device} { }
            ~Shaders() { vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr); vkDestroyPipeline(m_device, m_Pipeline, nullptr); }

            Shaders(const Shaders&) = delete;
            Shaders& operator=(const Shaders&) = delete;
            Shaders(Shaders&&) = delete;
            Shaders& operator=(Shaders&&) = delete;

            void createPipeline(const VkSampleCountFlagBits& msaaSample, const VkDescriptorSetLayout& descriptorSetLayout, const VkRenderPass& renderPass);
            //void createImguiPipeline(const VkRenderPass& renderPass);

            [[nodiscard]] const VkPipelineLayout& getPipelineLayout() const { return m_pipelineLayout; }
            //[[nodiscard]] const VkPipelineLayout& getImguiPipelineLayout() const { return m_imguiPipelineLayout; }
            [[nodiscard]] const VkPipeline& getPipeline() const { return m_Pipeline; }
            //[[nodiscard]] const VkPipeline& getImguiPipeline() const { return m_imguiPipeline; }

        private:

            void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule) const;

            const VkDevice& m_device;
            VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
            //VkPipelineLayout m_imguiPipelineLayout = VK_NULL_HANDLE;
            VkPipeline m_Pipeline = VK_NULL_HANDLE;
            // VkPipeline m_imguiPipeline = VK_NULL_HANDLE;

    }; // class Shaders

} // namespace ven
