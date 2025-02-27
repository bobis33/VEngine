///
/// @file SwapChain.hpp
/// @brief This file contains the SwapChain class
/// @namespace ven
///

#pragma once

#include "VEngine/Gfx/Backend/Device.hpp"

namespace ven {

    ///
    /// @class SwapChain
    /// @brief Class for swap chain
    /// @namespace ven
    ///
    class SwapChain {

        public:

            static constexpr uint8_t MAX_FRAMES_IN_FLIGHT = 2;

            SwapChain(const Device& device, const VkExtent2D& windowExtent) : m_device{device}, m_windowExtent{windowExtent} { init(); }
            ~SwapChain() { cleanupSwapChain(); }

            SwapChain(const SwapChain&) = delete;
            SwapChain& operator=(const SwapChain&) = delete;
            SwapChain(SwapChain&&) = delete;
            SwapChain& operator=(SwapChain&&) = delete;

            void init() { createSwapChain(); createImageViews(); createColorResources(); createDepthResources(); createRenderPass(); createFrameBuffers(); createSyncObjects(); }
            void createImageView(const VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkImageView& imageView) const;
            void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
            void cleanupSwapChain();
            [[nodiscard]] static VkFormat findDepthFormat(const Device& device) { return findSupportedFormat(device, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT); }
            [[nodiscard]] static bool hasStencilComponent(const VkFormat format) { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }
            [[nodiscard]] VkResult acquireNextImage(uint32_t& imageIndex, uint32_t currentFrame) const;

            [[nodiscard]] const VkSwapchainKHR& getSwapChain() const { return m_swapChain; }
            [[nodiscard]] VkFormat getFormat() const { return m_format; }
            [[nodiscard]] const VkExtent2D& getExtent() const { return m_extent; }
            [[nodiscard]] const std::vector<VkFramebuffer>& getSwapChainFrameBuffers() const { return m_swapChainFrameBuffers; }
            [[nodiscard]] const VkRenderPass& getRenderPass() const { return m_renderPass; }
            [[nodiscard]] const std::vector<VkSemaphore>& getImageAvailableSemaphores() const { return m_imageAvailableSemaphores; }
            [[nodiscard]] const std::vector<VkSemaphore>& getRenderFinishedSemaphores() const { return m_renderFinishedSemaphores; }
            [[nodiscard]] const std::vector<VkFence>& getInFlightFences() const { return m_inFlightFences; }

        private:

            void createSwapChain();
            void createImageViews();
            void createColorResources();
            void createDepthResources();
            void createFrameBuffers();
            void createRenderPass();
            void createSyncObjects();

            [[nodiscard]] static VkFormat findSupportedFormat(const Device& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

            const Device& m_device;
            VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
            std::vector<VkImage> m_images;
            std::vector<VkImageView> m_imageViews;
            VkFormat m_format = VK_FORMAT_UNDEFINED;
            VkExtent2D m_extent{};
            VkExtent2D m_windowExtent{};
            VkImage m_colorImage = VK_NULL_HANDLE;
            VkDeviceMemory m_colorImageMemory = VK_NULL_HANDLE;
            VkImageView m_colorImageView = VK_NULL_HANDLE;
            VkImage m_depthImage = VK_NULL_HANDLE;
            VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;
            VkImageView m_depthImageView = VK_NULL_HANDLE;
            std::vector<VkFramebuffer> m_swapChainFrameBuffers;
            VkRenderPass m_renderPass = VK_NULL_HANDLE;

            std::vector<VkSemaphore> m_imageAvailableSemaphores;
            std::vector<VkSemaphore> m_renderFinishedSemaphores;
            std::vector<VkFence> m_inFlightFences;

    }; // class SwapChain

} // namespace ven
