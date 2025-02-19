///
/// @file ABase.hpp
/// @brief This file contains the ARenderSystemBase class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/FrameInfo.hpp"
#include "VEngine/Gfx/Descriptors/SetLayout.hpp"
#include "VEngine/Gfx/Shaders.hpp"

namespace ven {

    ///
    /// @class ARenderSystemBase
    /// @brief Abstract class for render system base
    /// @namespace ven
    ///
    class ARenderSystemBase {

        public:

            explicit ARenderSystemBase(const Device& device) : m_device{device} {}
            virtual ~ARenderSystemBase() { vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr); }

            ARenderSystemBase(const ARenderSystemBase&) = delete;
            ARenderSystemBase& operator=(const ARenderSystemBase&) = delete;
            ARenderSystemBase(ARenderSystemBase&&) = delete;
            ARenderSystemBase& operator=(ARenderSystemBase&&) = delete;

            virtual void render(const FrameInfo &frameInfo) const = 0;

        protected:

            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout, uint32_t pushConstantSize);
            void createPipeline(VkRenderPass renderPass, const std::string &shadersVertPath, const std::string &shadersFragPath, bool isLight);

            [[nodiscard]] const Device& getDevice() const { return m_device; }
            [[nodiscard]] const VkPipelineLayout& getPipelineLayout() const { return m_pipelineLayout; }
            [[nodiscard]] const std::unique_ptr<Shaders>& getShaders() const { return m_shaders; }

            std::unique_ptr<DescriptorSetLayout> renderSystemLayout;

        private:

            const Device &m_device;
            VkPipelineLayout m_pipelineLayout{nullptr};
            std::unique_ptr<Shaders> m_shaders;

    }; // class ARenderSystemBase

} // namespace ven
