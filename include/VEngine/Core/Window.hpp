///
/// @file Window.hpp
/// @brief This file contains the Window class
/// @namespace ven
///

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Utils/ErrorHandling.hpp"

namespace ven {

    ///
    /// @class Window
    /// @brief Class for window
    /// @namespace ven
    ///
    class Window {

        public:

            static constexpr uint16_t DEFAULT_WIDTH = 1920;
            static constexpr uint16_t DEFAULT_HEIGHT = 1080;

            explicit Window(const uint16_t width = DEFAULT_WIDTH, const uint16_t height = DEFAULT_HEIGHT) : m_window(createWindow(width, height, "VEngine")) { setWindowIcon("assets/icons/icon64x64.png"); }
            ~Window() { glfwTerminate(); }

            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
            Window(Window&&) = delete;
            Window& operator=(Window&&) = delete;

            void createWindowSurface(const VkInstance& instance, VkSurfaceKHR* surface) const { if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) { throw utl::THROW_ERROR("Failed to create window surface"); } }
            static void setFullscreen(bool fullscreen, uint16_t width, uint16_t height);
            [[nodiscard]] bool wasWindowResized() const { return m_frameBufferResized; }
            void resetWindowResizedFlag() { m_frameBufferResized = false; }
            static void pollEvents() { glfwPollEvents(); }
            [[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(m_window) != 0; }
            static void waitEvents() { glfwWaitEvents(); }

            [[nodiscard]] VkExtent2D getExtent() const { int width = 0; int height = 0; glfwGetFramebufferSize(m_window, &width, &height); return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
            [[nodiscard]] GLFWwindow* getGLFWWindow() const { return m_window; }
            void getFrameBufferSize(int& width, int& height) const { glfwGetFramebufferSize(m_window, &width, &height); }
            [[nodiscard]] static const char **getRequiredInstanceExtensions(uint32_t *count) { return glfwGetRequiredInstanceExtensions(count); }

        private:

            [[nodiscard]] GLFWwindow* createWindow(uint16_t width, uint16_t height, const std::string &title);
            void setWindowIcon(const std::string& path) const;
            static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) { static_cast<Window *>(glfwGetWindowUserPointer(window))->m_frameBufferResized = true; }

            GLFWwindow* m_window = nullptr;
            bool m_frameBufferResized = false;

    }; // class Window

} // namespace ven
