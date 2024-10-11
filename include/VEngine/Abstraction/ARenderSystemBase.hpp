///
/// @file ARenderSystemBase.hpp
/// @brief This file contains the ARenderSystemBase class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"


namespace ven {

    ///
    /// @class ARenderSystemBase
    /// @brief Abstract class for render system base
    /// @namespace ven
    ///
    class ARenderSystemBase {

        public:
            explicit ARenderSystemBase(Device& device)
                : m_device{device} {}

            ~ARenderSystemBase() { vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr); }

        protected:

            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout, uint32_t pushConstantSize);
            void createPipeline(VkRenderPass renderPass, std::string shadersVertPath, std::string shadersFragPath, bool isLight);

            [[nodiscard]] Device& getDevice() const { return m_device; }
            [[nodiscard]] VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }
            [[nodiscard]] Shaders& getShaders() const { return *m_shaders; }

        private:

            Device &m_device;
            VkPipelineLayout m_pipelineLayout{nullptr};
            std::unique_ptr<Shaders> m_shaders;

    }; // class RenderSystemBase

} // namespace ven
