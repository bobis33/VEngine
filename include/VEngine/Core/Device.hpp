///
/// @file Device.hpp
/// @brief This file contains the Device class
/// @namespace ven
///

#pragma once

#include <vector>
#include <stdexcept>

#include "VEngine/Core/Window.hpp"

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

    ///
    /// @class Device
    /// @brief Class for device
    /// @namespace ven
    ///
    class Device {

        public:

            #ifdef NDEBUG
                const bool enableValidationLayers = false;
            #else
                const bool enableValidationLayers = true;
            #endif

            explicit Device(Window &window);
            ~Device();

            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;

            [[nodiscard]] VkCommandPool getCommandPool() const { return m_commandPool; }
            [[nodiscard]] VkDevice device() const { return m_device; }
            [[nodiscard]] VkSurfaceKHR surface() const { return m_surface; }
            [[nodiscard]] VkQueue graphicsQueue() const { return m_graphicsQueue; }
            [[nodiscard]] VkQueue presentQueue() const { return m_presentQueue; }

            [[nodiscard]] SwapChainSupportDetails getSwapChainSupport() const { return querySwapChainSupport(m_physicalDevice); }
            [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
            [[nodiscard]] QueueFamilyIndices findPhysicalQueueFamilies() const { return findQueueFamilies(m_physicalDevice); }
            [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
            [[nodiscard]] VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
            [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
            [[nodiscard]] VkPhysicalDeviceProperties getProperties() const { return m_properties; }
            [[nodiscard]] VkInstance getInstance() const { return m_instance; }

            // Buffer Helper Functions
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const;
            [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() const;
            void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
            void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

            void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) const;
            void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1, uint32_t layerCount = 1) const;

        private:

            void createInstance();
            void setupDebugMessenger();
            void createSurface() { m_window.createWindowSurface(m_instance, &m_surface); };
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

            Window &m_window;
            VkDebugUtilsMessengerEXT m_debugMessenger;
            VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
            VkCommandPool m_commandPool;
            VkDevice m_device;
            VkInstance m_instance;
            VkSurfaceKHR m_surface;
            VkQueue m_graphicsQueue;
            VkQueue m_presentQueue;
            VkPhysicalDeviceProperties m_properties;

            const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
            const std::vector<const char *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    }; // class Device

}  // namespace ven