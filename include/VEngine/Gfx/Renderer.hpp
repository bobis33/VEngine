///
/// @file Renderer.hpp
/// @brief This file contains the Renderer class
/// @namespace ven
///

#pragma once

#include "VEngine/Gfx/Resources/Model.hpp"
#include "VEngine/Gfx/Shaders.hpp"
#include "VEngine/Gui/Gui.hpp"

namespace ven {

    static constexpr uint8_t OFFSET = 16;
    struct UniformBufferObject {
        alignas(OFFSET) glm::mat4 model;
        alignas(OFFSET) glm::mat4 view;
        alignas(OFFSET) glm::mat4 proj;
        alignas(OFFSET) glm::vec3 ambientColor;
    };

    ///
    /// @class Renderer
    /// @brief Class for renderer
    /// @namespace ven
    ///
    class Renderer {

        public:

            static constexpr VkDeviceSize UNIFORM_BUFFER_SIZE{sizeof(UniformBufferObject)};

            explicit Renderer(const Device &device, Window& window, std::vector<Model>& models) : m_device(device), m_window(window),
                                                                      m_swapChain(m_device, window.getExtent()), m_shadersModule(m_device.getVkDevice()),
                                                                      m_gui(m_device, m_camera, window.getGLFWWindow(), m_swapChain.getRenderPass(), models, m_clearValues, m_ambientColor) {  }

            ~Renderer();

            Renderer(const Renderer &) = delete;
            Renderer& operator=(const Renderer &) = delete;
            Renderer(Renderer &&) = delete;
            Renderer& operator=(Renderer &&) = delete;

            void createCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers) const;
            void recreateSwapChain();
            void updateUniformBuffer(void* uniformBufferMapped, std::vector<Model>& models) const;
            void recordCommandBuffer(uint32_t imageIndex, uint32_t indiceSize, const VkDescriptorSet* descriptorSet, const VkCommandBuffer& commandBuffer, const VkBuffer& indexBuffer, const VkBuffer& vertexBuffer);

            [[nodiscard]] const SwapChain& getSwapChain() const { return m_swapChain; }
            [[nodiscard]] Camera& getCamera() { return m_camera; }
            [[nodiscard]] Shaders& getShadersModule() { return m_shadersModule; }

        private:

            std::array<VkClearValue, 2> m_clearValues{
                VkClearValue{ .color = {0.0F, 0.0F, 0.0F, 1.0F}},
                VkClearValue{.depthStencil = {1.0F, 0}}};
            glm::vec3 m_ambientColor{1.0F, 1.0F, 1.0F};
            const Device& m_device;
            Window& m_window;
            SwapChain m_swapChain;
            Shaders m_shadersModule;
            Camera m_camera;
            Gui m_gui;

    }; // class Renderer

} // namespace ven
