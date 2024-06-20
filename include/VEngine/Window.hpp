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

            Window(int width, int height, const std::string &title) : m_window(createWindow(width, height, title)), m_width(width), m_height(height) {};
            ~Window() { glfwDestroyWindow(m_window); glfwTerminate(); m_window = nullptr;};

            GLFWwindow* createWindow(int width, int height, const std::string &title);
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

            [[nodiscard]] GLFWwindow* getGLFWindow() const { return m_window; };

            [[nodiscard]] VkExtent2D getExtent() const { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; };
            bool wasWindowResized() { return m_framebufferResized; }
            void resetWindowResizedFlag() { m_framebufferResized = false; }

        private:

            static void framebufferResizeCallback(GLFWwindow* windowp, int width, int height);

            GLFWwindow* m_window{nullptr};
            int m_width;
            int m_height;

            bool m_framebufferResized = false;

    }; // class Window

} // namespace ven
