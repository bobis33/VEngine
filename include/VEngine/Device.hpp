///
/// @file Device.hpp
/// @brief This file contains the Device class
/// @namespace ven
///

#pragma once

#include <vector>

#include "VEngine/Window.hpp"

namespace ven {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily{};
        uint32_t presentFamily{};
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        [[nodiscard]] bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class Device {

        public:

            #ifdef NDEBUG
                const bool enableValidationLayers = false;
            #else
                const bool enableValidationLayers = true;
            #endif

            explicit Device(Window &window);
            ~Device();

            Device(const Device &) = delete;
            Device& operator=(const Device &) = delete;
            Device(Device &&) = delete;
            Device &operator=(Device &&) = delete;

            [[nodiscard]] VkCommandPool getCommandPool() const { return commandPool; }
            [[nodiscard]] VkDevice device() const { return device_; }
            [[nodiscard]] VkSurfaceKHR surface() const { return surface_; }
            [[nodiscard]] VkQueue graphicsQueue() const { return graphicsQueue_; }
            [[nodiscard]] VkQueue presentQueue() const { return presentQueue_; }

        [[nodiscard]] SwapChainSupportDetails getSwapChainSupport() const { return querySwapChainSupport(physicalDevice); }
        [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertiesp) const;
        [[nodiscard]] QueueFamilyIndices findPhysicalQueueFamilies() const { return findQueueFamilies(physicalDevice); }
        [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

            // Buffer Helper Functions
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags propertiesp, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const;
            [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() const;
            void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
            void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

            void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) const;

            VkPhysicalDeviceProperties m_properties;

            [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
            [[nodiscard]] VkQueue getGraphicsQueue() const { return graphicsQueue_; }

        private:

            void createInstance();
            void setupDebugMessenger();
            void createSurface() { m_window.createWindowSurface(instance, &surface_); };
            void pickPhysicalDevice();
            void createLogicalDevice();
            void createCommandPool();

            // helper functions
            bool isDeviceSuitable(VkPhysicalDevice device) const;
            [[nodiscard]] std::vector<const char *> getRequiredExtensions() const;
            [[nodiscard]] bool checkValidationLayerSupport() const;
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
            static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
            void hasGlfwRequiredInstanceExtensions() const;
            bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;

            VkInstance instance;
            VkDebugUtilsMessengerEXT debugMessenger;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            Window &m_window;
            VkCommandPool commandPool;

            VkDevice device_;
            VkSurfaceKHR surface_;
            VkQueue graphicsQueue_;
            VkQueue presentQueue_;

            const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
            const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    }; // class Device

}  // namespace ven
