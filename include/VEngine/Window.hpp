/*
** @file Window.hpp
** @brief This file contains the Window class
** @namespace ven
*/

#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ven {

    class Window {

        public:

            Window(const uint32_t width, const uint32_t height, const std::string &title) : m_window(createWindow(width, height, title)), m_width(width), m_height(height) {};
            ~Window() { glfwDestroyWindow(m_window); glfwTerminate(); m_window = nullptr;};

            [[nodiscard]] GLFWwindow* createWindow(uint32_t width, uint32_t height, const std::string &title);
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

            [[nodiscard]] GLFWwindow* getGLFWindow() const { return m_window; };

            [[nodiscard]] VkExtent2D getExtent() const { return {m_width, m_height}; };
            [[nodiscard]] bool wasWindowResized() const { return m_framebufferResized; }
            void resetWindowResizedFlag() { m_framebufferResized = false; }

        private:

            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

            GLFWwindow* m_window{nullptr};
            uint32_t m_width;
            uint32_t m_height;

            bool m_framebufferResized = false;

    }; // class Window

} // namespace ven
