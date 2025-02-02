///
/// @file Renderer.hpp
/// @brief This file contains the Renderer class
/// @namespace ven
///

#pragma once

#include <cassert>
#include <array>

#include "VEngine/Gfx/SwapChain.hpp"

namespace ven {

    static constexpr VkClearColorValue DEFAULT_CLEAR_COLOR = {{0.0F, 0.0F, 0.0F, 1.0F}};
    static constexpr VkClearDepthStencilValue DEFAULT_CLEAR_DEPTH = {1.0F, 0};

    ///
    /// @class Renderer
    /// @brief Class for renderer
    /// @namespace ven
    ///
    class Renderer {

        public:

            Renderer(const Window &window, const Device &device) : m_window{window}, m_device{device} { recreateSwapChain(); createCommandBuffers(); }
            ~Renderer() { freeCommandBuffers(); }

            Renderer(const Renderer &) = delete;
            Renderer& operator=(const Renderer &) = delete;
            Renderer(Renderer &&) = delete;
            Renderer& operator=(Renderer &&) = delete;

            [[nodiscard]] VkRenderPass getSwapChainRenderPass() const { return m_swapChain->getRenderPass(); }
            [[nodiscard]] float getAspectRatio() const { return m_swapChain->extentAspectRatio(); }
            [[nodiscard]] bool isFrameInProgress() const { return m_isFrameStarted; }
            [[nodiscard]] const VkCommandBuffer& getCurrentCommandBuffer() const { assert(isFrameInProgress() && "cannot get command m_buffer when frame not in progress"); return m_commandBuffers[static_cast<unsigned long>(m_currentFrameIndex)]; }
            [[nodiscard]] const Window& getWindow() const { return m_window; }
            [[nodiscard]] unsigned long getFrameIndex() const { assert(isFrameInProgress() && "cannot get frame index when frame not in progress"); return m_currentFrameIndex; }
            [[nodiscard]] std::array<float, 4> getClearColor() const { return {
                m_clearValues[0].color.float32[0],
                m_clearValues[0].color.float32[1],
                m_clearValues[0].color.float32[2],
                m_clearValues[0].color.float32[3]
            };}


            void setClearValue(const VkClearColorValue clearColorValue = DEFAULT_CLEAR_COLOR, const VkClearDepthStencilValue clearDepthValue = DEFAULT_CLEAR_DEPTH) { m_clearValues[0].color = clearColorValue; m_clearValues[1].depthStencil = clearDepthValue; }
            VkCommandBuffer beginFrame();
            void endFrame();
            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer) const;
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

        private:

            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

            const Window &m_window;
            const Device &m_device;
            std::unique_ptr<SwapChain> m_swapChain;
            std::vector<VkCommandBuffer> m_commandBuffers;
            std::array<VkClearValue, 2> m_clearValues{DEFAULT_CLEAR_COLOR, 1.0F, 0.F};

            uint32_t m_currentImageIndex{0};
            unsigned long m_currentFrameIndex{0};
            bool m_isFrameStarted{false};

    }; // class Renderer

} // namespace ven
