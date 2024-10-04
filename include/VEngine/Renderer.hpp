///
/// @file Renderer.hpp
/// @brief This file contains the Renderer class
/// @namespace ven
///

#pragma once

#include <memory>
#include <cassert>

#include <vulkan/vulkan.h>

#include "VEngine/Window.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/SwapChain.hpp"

namespace ven {

    class Renderer {

    public:

        Renderer(Window &window, Device &device) : m_window{window}, m_device{device} { recreateSwapChain(); createCommandBuffers(); }
        ~Renderer() { freeCommandBuffers(); }

        Renderer(const Renderer &) = delete;
        Renderer& operator=(const Renderer &) = delete;

        [[nodiscard]] VkRenderPass getSwapChainRenderPass() const { return m_swapChain->getRenderPass(); }
        [[nodiscard]] float getAspectRatio() const { return m_swapChain->extentAspectRatio(); }
        [[nodiscard]] bool isFrameInProgress() const { return m_isFrameStarted; }
        [[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const { assert(isFrameInProgress() && "cannot get command m_buffer when frame not in progress"); return m_commandBuffers[static_cast<unsigned long>(m_currentFrameIndex)]; }

        [[nodiscard]] int getFrameIndex() const { assert(isFrameInProgress() && "cannot get frame index when frame not in progress"); return m_currentFrameIndex; }
        [[nodiscard]] std::array<float, 4> getClearColor() const { return {
                    m_clearValues[0].color.float32[0],
                    m_clearValues[0].color.float32[1],
                    m_clearValues[0].color.float32[2],
                    m_clearValues[0].color.float32[3]
            };
        }

        void setClearColor(const std::array<float, 4>& color) { m_clearValues[0].color = {{color[0], color[1], color[2], color[3]}}; }
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window &m_window;
        Device &m_device;
        std::unique_ptr<SwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::array<VkClearValue, 2> m_clearValues{};

        uint32_t m_currentImageIndex{0};
        int m_currentFrameIndex{0};
        bool m_isFrameStarted{false};

    }; // class Renderer

} // namespace ven
