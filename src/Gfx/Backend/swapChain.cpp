#include <limits>

#include "VEngine/Gfx/Backend/SwapChain.hpp"

static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    // VK_PRESENT_MODE_IMMEDIATE_KHR = 0, = Immediate
    // VK_PRESENT_MODE_MAILBOX_KHR = 1, = Triple Buffering
    // VK_PRESENT_MODE_FIFO_KHR = 2, = V-Sync
    // VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3, = V-Sync (Relaxed)
    // VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR = 1000111000, = Shared Demand Refresh
    // VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR = 1000111001, = Shared Continuous Refresh
    // VK_PRESENT_MODE_FIFO_LATEST_READY_EXT = 1000361000, = V-Sync (Latest Ready)
    // VK_PRESENT_MODE_MAX_ENUM_KHR = 0x7FFFFFFF = Max Enum
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
        if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D& extent) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    VkExtent2D actualExtent = extent;
    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    return actualExtent;
}

VkResult ven::SwapChain::acquireNextImage(uint32_t &imageIndex, const uint32_t currentFrame) const {
    if (vkWaitForFences(m_device.getVkDevice(), 1, &m_inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to wait for fences!");
    }
    return vkAcquireNextImageKHR(m_device.getVkDevice(), m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
}

VkFormat ven::SwapChain::findSupportedFormat(const Device& device, const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) {
    for (const VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice(), format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    throw utl::THROW_ERROR("failed to find supported format!");
}

static void createAttachmentDescription(VkAttachmentDescription& attachmentDescription, const VkFormat& format, const VkSampleCountFlagBits& samples, const VkAttachmentLoadOp& loadOp, const VkAttachmentStoreOp& storeOp, const VkImageLayout& finalLayout) {
    attachmentDescription.flags = 0;
    attachmentDescription.format = format;
    attachmentDescription.samples = samples;
    attachmentDescription.loadOp = loadOp;
    attachmentDescription.storeOp = storeOp;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = finalLayout;
}

void ven::SwapChain::createSwapChain() {
    auto [capabilities, formats, presentModes] = m_device.querySwapChainSupport(m_device.getPhysicalDevice());
    const auto [format, colorSpace] = chooseSwapSurfaceFormat(formats);
    const VkExtent2D extent = chooseSwapExtent(capabilities, m_windowExtent);
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_device.getVkSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    auto [graphicsFamily, presentFamily] = m_device.findQueueFamilies(m_device.getPhysicalDevice());
    const std::array queueFamilyIndices = {graphicsFamily.value(), presentFamily.value()};
    if (graphicsFamily != presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = chooseSwapPresentMode(presentModes);
    createInfo.clipped = VK_TRUE;
    if (vkCreateSwapchainKHR(m_device.getVkDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to create swap chain!");
    }
    vkGetSwapchainImagesKHR(m_device.getVkDevice(), m_swapChain, &imageCount, nullptr);
    m_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device.getVkDevice(), m_swapChain, &imageCount, m_images.data());
    m_format = format;
    m_extent = extent;
}


void ven::SwapChain::createImageView(const VkImage& image, const VkFormat format, const VkImageAspectFlags aspectFlags, const uint32_t mipLevels, VkImageView& imageView) const {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(m_device.getVkDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to create image view");
    }
}

void ven::SwapChain::createImageViews() {
    m_imageViews.resize(m_images.size());
    for (uint32_t i = 0; i < m_images.size(); i++) {
        createImageView( m_images[i], m_format, VK_IMAGE_ASPECT_COLOR_BIT, 1, m_imageViews[i]);
    }
}

void ven::SwapChain::createImage(const uint32_t width, const uint32_t height, const uint32_t mipLevels, const VkSampleCountFlagBits numSamples, const VkFormat format, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateImage(m_device.getVkDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to create image!");
    }
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_device.getVkDevice(), image, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = m_device.findMemoryType(memRequirements.memoryTypeBits, properties);
    if (vkAllocateMemory(m_device.getVkDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to allocate image memory!");
    }
    vkBindImageMemory(m_device.getVkDevice(), image, imageMemory, 0);
}

void ven::SwapChain::createColorResources() {
    const VkFormat colorFormat = m_format;
    createImage(m_extent.width, m_extent.height, 1, m_device.getMsaaSamples(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_colorImage, m_colorImageMemory);
    createImageView(m_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, m_colorImageView);
}

void ven::SwapChain::createDepthResources() {
    const VkFormat depthFormat = findDepthFormat(m_device);
    createImage(m_extent.width, m_extent.height, 1, m_device.getMsaaSamples(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
    createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, m_depthImageView);
}

void ven::SwapChain::createFrameBuffers() {
    m_swapChainFrameBuffers.resize(m_imageViews.size());
    for (size_t i = 0; i < m_imageViews.size(); i++) {
        std::array attachments = {
            m_colorImageView,
            m_depthImageView,
            m_imageViews[i]
        };
        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = m_renderPass;
        frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameBufferInfo.pAttachments = attachments.data();
        frameBufferInfo.width = m_extent.width;
        frameBufferInfo.height = m_extent.height;
        frameBufferInfo.layers = 1;
        if (vkCreateFramebuffer(m_device.getVkDevice(), &frameBufferInfo, nullptr, &m_swapChainFrameBuffers[i]) != VK_SUCCESS) {
            throw utl::THROW_ERROR("failed to create frame buffer!");
        }
    }
}

void ven::SwapChain::createRenderPass() {
    constexpr VkAttachmentReference colorAttachmentRef{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    constexpr VkAttachmentReference depthAttachmentRef{ .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
    constexpr VkAttachmentReference colorAttachmentResolveRef{ .attachment = 2, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentDescription colorAttachment{};
    VkAttachmentDescription depthAttachment{};
    VkAttachmentDescription colorAttachmentResolve{};
    createAttachmentDescription(colorAttachment, m_format, m_device.getMsaaSamples(), VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    createAttachmentDescription(depthAttachment, findDepthFormat(m_device), m_device.getMsaaSamples(), VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    createAttachmentDescription(colorAttachmentResolve, m_format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    const std::array attachments = {colorAttachment, depthAttachment, colorAttachmentResolve };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    if (vkCreateRenderPass(m_device.getVkDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to create render pass!");
    }
}

void ven::SwapChain::createSyncObjects() {
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(m_device.getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(m_device.getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(m_device.getVkDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            throw utl::THROW_ERROR("failed to create synchronization objects for a frame!");
        }
    }
}

void ven::SwapChain::cleanupSwapChain() {
    const VkDevice& device = m_device.getVkDevice();
    for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, m_inFlightFences[i], nullptr);
    }
    vkDestroyImageView(device, m_depthImageView, nullptr);
    vkDestroyImage(device, m_depthImage, nullptr);
    vkFreeMemory(device, m_depthImageMemory, nullptr);
    vkDestroyImageView(device, m_colorImageView, nullptr);
    vkDestroyImage(device, m_colorImage, nullptr);
    vkFreeMemory(device, m_colorImageMemory, nullptr);
    for (auto *const frameBuffer : m_swapChainFrameBuffers) {
        vkDestroyFramebuffer(device, frameBuffer, nullptr);
    }
    for (auto *const imageView : m_imageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(device, m_swapChain, nullptr);
    vkDestroyRenderPass(device, m_renderPass, nullptr);
}
