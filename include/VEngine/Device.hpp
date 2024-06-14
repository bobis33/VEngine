/*
** @file Device.hpp
** @brief This file contains the Device class
** @namespace ven
*/

#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <vulkan/vulkan.h>

namespace ven {

    class Device {

        public:

            Device(VkInstance instance, VkSurfaceKHR surface) : m_instance(instance), m_surface(surface) { pickPhysicalDevice(); createLogicalDevice(); }
            ~Device() { vkDestroyDevice(m_device, nullptr); }

            [[nodiscard]] VkDevice getDevice() const { return m_device; }
            [[nodiscard]] VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
            [[nodiscard]] VkQueue getPresentQueue() const { return m_presentQueue; }

        private:
            VkDevice m_device{VK_NULL_HANDLE};
            VkQueue m_graphicsQueue{VK_NULL_HANDLE};
            VkQueue m_presentQueue{VK_NULL_HANDLE};

            VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
            VkInstance m_instance{nullptr};
            VkSurfaceKHR m_surface{nullptr};

            void pickPhysicalDevice();
            void createLogicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice device);
            static std::vector<const char*> getRequiredExtensions() { return { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; }
            static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

            struct QueueFamilyIndices {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
            };

            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    }; // class Device

} // namespace ven
