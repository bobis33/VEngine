///
/// @file SwapChain.hpp
/// @brief This file contains the Shader class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Core/Device.hpp"

namespace ven {

    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    ///
    /// @class SwapChain
    /// @brief Class for swap chain
    /// @namespace ven
    ///
    class SwapChain {

        public:

            SwapChain(const Device &deviceRef, const VkExtent2D windowExtentRef) : m_device{deviceRef}, m_windowExtent{windowExtentRef} { init(); }
            SwapChain(const Device &deviceRef, const VkExtent2D windowExtentRef, std::shared_ptr<SwapChain> previous) : m_device{deviceRef}, m_windowExtent{windowExtentRef}, m_oldSwapChain{std::move(previous)} { init(); m_oldSwapChain = nullptr; }
            ~SwapChain();

            SwapChain(const SwapChain &) = delete;
            SwapChain& operator=(const SwapChain &) = delete;
            SwapChain(SwapChain &&) = delete;
            SwapChain& operator=(SwapChain &&) = delete;

            [[nodiscard]] const VkFramebuffer& getFrameBuffer(const unsigned long index) const { return m_swapChainFrameBuffers[index]; }
            [[nodiscard]] const VkRenderPass& getRenderPass() const { return m_renderPass; }
            [[nodiscard]] const VkImageView& getImageView(const int index) const { return m_swapChainImageViews[static_cast<unsigned long>(index)]; }
            [[nodiscard]] size_t imageCount() const { return m_swapChainImages.size(); }
            [[nodiscard]] const VkFormat& getSwapChainImageFormat() const { return m_swapChainImageFormat; }
            [[nodiscard]] const VkExtent2D& getSwapChainExtent() const { return m_swapChainExtent; }
            [[nodiscard]] uint32_t width() const { return m_swapChainExtent.width; }
            [[nodiscard]] uint32_t height() const { return m_swapChainExtent.height; }
            [[nodiscard]] float extentAspectRatio() const { return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height); }
            [[nodiscard]] VkFormat findDepthFormat() const;
            [[nodiscard]] VkResult acquireNextImage(uint32_t *imageIndex) const;
            [[nodiscard]] VkResult submitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);
            [[nodiscard]] bool compareSwapFormats(const SwapChain &swapChain) const { return m_swapChainImageFormat == swapChain.m_swapChainImageFormat && m_swapChainDepthFormat == swapChain.m_swapChainDepthFormat; }

        private:

            void init();
            void createSwapChain();
            void createImageViews();
            void createDepthResources();
            void createRenderPass();
            void createFrameBuffers();
            void createSyncObjects();
            [[nodiscard]] static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
            [[nodiscard]] static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
            [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

            std::shared_ptr<SwapChain> m_oldSwapChain;
            const Device &m_device;
            VkFormat m_swapChainImageFormat{};
            VkFormat m_swapChainDepthFormat{};
            VkExtent2D m_swapChainExtent{};

            std::vector<VkFramebuffer> m_swapChainFrameBuffers;
            VkRenderPass m_renderPass{};

            std::vector<VkImage> m_depthImages;
            std::vector<VkDeviceMemory> m_depthImageMemory;
            std::vector<VkImageView> m_depthImageViews;
            std::vector<VkImage> m_swapChainImages;
            std::vector<VkImageView> m_swapChainImageViews;

            VkExtent2D m_windowExtent;

            VkSwapchainKHR m_swapChain{};

            std::vector<VkSemaphore> m_imageAvailableSemaphores;
            std::vector<VkSemaphore> m_renderFinishedSemaphores;
            std::vector<VkFence> m_inFlightFences;
            std::vector<VkFence> m_imagesInFlight;
            size_t m_currentFrame{0};

    }; // class SwapChain

}  // namespace ven
