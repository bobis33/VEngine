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

            Window(int width, int height, const std::string &title) : m_window(createWindow(width, height, title)) {};
            ~Window() { glfwDestroyWindow(m_window); glfwTerminate(); m_window = nullptr;};

            static GLFWwindow* createWindow(int width, int height, const std::string &title);
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

            [[nodiscard]] GLFWwindow* getGLFWindow() const { return m_window; };

            VkExtent2D getExtent() const {
                int width, height;
                glfwGetFramebufferSize(m_window, &width, &height);
                return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            }

        private:

            GLFWwindow* m_window{nullptr};

    }; // class Window

} // namespace ven
