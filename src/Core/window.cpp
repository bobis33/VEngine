#include <stdexcept>

#include "VEngine/Core/Window.hpp"

GLFWwindow* ven::Window::createWindow(const uint32_t width, const uint32_t height, const std::string &title)
{
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    return window;
}

void ven::Window::createWindowSurface(const VkInstance instance, VkSurfaceKHR *surface) const
{
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}

void ven::Window::framebufferResizeCallback(GLFWwindow *window, const int width, const int height)
{
    auto *app = static_cast<Window *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
    app->m_width = static_cast<uint32_t>(width);
    app->m_height = static_cast<uint32_t>(height);
}

void ven::Window::setFullscreen(const bool fullscreen, const uint32_t width, const uint32_t height)
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    /*
    if (fullscreen) {
        glfwSetWindowMonitor(m_window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        // To restore a window that was originally windowed to its original size and position,
        // save these before making it full screen and then pass them in as above
        glfwSetWindowMonitor(m_window, nullptr, 0, 0, static_cast<int>(width), static_cast<int>(height), mode->refreshRate);

    }

    m_width = width;
    m_height = height;
    */
}
