///
/// @file SwapChain.hpp
/// @brief This file contains the Shader class
/// @namespace ven
///

#pragma once

#include <vulkan/vulkan.h>
#include <memory>

#include "VEngine/Device.hpp"

namespace ven {

    class SwapChain {

        public:

            static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

            SwapChain(Device &deviceRef, const VkExtent2D windowExtentRef) : device{deviceRef}, windowExtent{windowExtentRef} { init(); }
            SwapChain(Device &deviceRef, const VkExtent2D windowExtentRef, std::shared_ptr<SwapChain> previous) : device{deviceRef}, windowExtent{windowExtentRef}, oldSwapChain{std::move(previous)} { init(); oldSwapChain = nullptr; }
            ~SwapChain();

            SwapChain(const SwapChain &) = delete;
            SwapChain& operator=(const SwapChain &) = delete;

            [[nodiscard]] VkFramebuffer getFrameBuffer(const unsigned long index) const { return swapChainFramebuffers[index]; }
            [[nodiscard]] VkRenderPass getRenderPass() const { return renderPass; }
            [[nodiscard]] VkImageView getImageView(const int index) const { return swapChainImageViews[static_cast<unsigned long>(index)]; }
            [[nodiscard]] size_t imageCount() const { return swapChainImages.size(); }
            [[nodiscard]] VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
            [[nodiscard]] VkExtent2D getSwapChainExtent() const { return m_swapChainExtent; }
            [[nodiscard]] uint32_t width() const { return m_swapChainExtent.width; }
            [[nodiscard]] uint32_t height() const { return m_swapChainExtent.height; }

            [[nodiscard]] float extentAspectRatio() const { return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height); }
            VkFormat findDepthFormat() const;

            VkResult acquireNextImage(uint32_t *imageIndex) const;
            VkResult submitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);

            [[nodiscard]] bool compareSwapFormats(const SwapChain &swapChainp) const {
                return swapChainImageFormat == swapChainp.swapChainImageFormat && swapChainDepthFormat == swapChainp.swapChainDepthFormat;
            }

        private:

            void init();
            void createSwapChain();
            void createImageViews();
            void createDepthResources();
            void createRenderPass();
            void createFramebuffers();
            void createSyncObjects();

            static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
            static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

            VkFormat swapChainImageFormat{};
            VkFormat swapChainDepthFormat{};
            VkExtent2D m_swapChainExtent{};

            std::vector<VkFramebuffer> swapChainFramebuffers;
            VkRenderPass renderPass{};

            std::vector<VkImage> depthImages;
            std::vector<VkDeviceMemory> depthImageMemorys;
            std::vector<VkImageView> depthImageViews;
            std::vector<VkImage> swapChainImages;
            std::vector<VkImageView> swapChainImageViews;

            Device &device;
            VkExtent2D windowExtent;

            VkSwapchainKHR swapChain{};
            std::shared_ptr<SwapChain> oldSwapChain;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;
            std::vector<VkFence> imagesInFlight;
            size_t currentFrame = 0;

    }; // class SwapChain

}  // namespace ven
