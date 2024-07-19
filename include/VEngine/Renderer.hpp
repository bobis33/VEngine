/*
** @file Renderer.hpp
** @brief This file contains the Renderer class
** @namespace ven
*/

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

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer) const;
        static void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window &m_window;
        Device &m_device;
        std::unique_ptr<SwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;

        uint32_t m_currentImageIndex{0};
        int m_currentFrameIndex{0};
        bool m_isFrameStarted{false};

    }; // class Renderer

} // namespace ven
