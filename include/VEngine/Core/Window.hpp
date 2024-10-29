///
/// @file Window.hpp
/// @brief This file contains the Window class
/// @namespace ven
///

#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ven {

    static constexpr uint32_t DEFAULT_WIDTH = 1920;
    static constexpr uint32_t DEFAULT_HEIGHT = 1080;
    static constexpr std::string_view DEFAULT_TITLE = "VEngine";

    ///
    /// @class Window
    /// @brief Class for window
    /// @namespace ven
    ///
    class Window {

        public:

            explicit Window(const uint32_t width = DEFAULT_WIDTH, const uint32_t height = DEFAULT_HEIGHT, const std::string &title = DEFAULT_TITLE.data()) : m_window(createWindow(width, height, title)), m_width(width), m_height(height) {}
            ~Window() { glfwDestroyWindow(m_window); glfwTerminate(); m_window = nullptr;};

            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            [[nodiscard]] GLFWwindow* createWindow(uint32_t width, uint32_t height, const std::string &title);
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;

            [[nodiscard]] GLFWwindow* getGLFWindow() const { return m_window; }

            [[nodiscard]] VkExtent2D getExtent() const { return {m_width, m_height}; }
            [[nodiscard]] bool wasWindowResized() const { return m_framebufferResized; }
            void resetWindowResizedFlag() { m_framebufferResized = false; }

            void setFullscreen(bool fullscreen, uint32_t width, uint32_t height);

        private:

            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

            GLFWwindow* m_window{nullptr};
            uint32_t m_width{DEFAULT_WIDTH};
            uint32_t m_height{DEFAULT_HEIGHT};

            bool m_framebufferResized = false;

    }; // class Window

} // namespace ven
