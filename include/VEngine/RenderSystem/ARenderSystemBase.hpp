///
/// @file ARenderSystemBase.hpp
/// @brief This file contains the ARenderSystemBase class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Descriptors/DescriptorSetLayout.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    ///
    /// @class ARenderSystemBase
    /// @brief Abstract class for render system base
    /// @namespace ven
    ///
    class ARenderSystemBase {

        public:

            explicit ARenderSystemBase(Device& device) : m_device{device} {}
            virtual ~ARenderSystemBase() { vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr); }

            virtual void render(const FrameInfo &frameInfo) const = 0;

        protected:

            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout, uint32_t pushConstantSize);
            void createPipeline(VkRenderPass renderPass, const std::string &shadersVertPath, const std::string &shadersFragPath, bool isLight);

            [[nodiscard]] Device& getDevice() const { return m_device; }
            [[nodiscard]] VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }
            [[nodiscard]] const std::unique_ptr<Shaders>& getShaders() const { return m_shaders; }

            std::unique_ptr<DescriptorSetLayout> renderSystemLayout;

        private:

            Device &m_device;
            VkPipelineLayout m_pipelineLayout{nullptr};
            std::unique_ptr<Shaders> m_shaders;


    }; // class ARenderSystemBase

} // namespace ven
