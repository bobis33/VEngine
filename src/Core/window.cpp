#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Utils/Image.hpp"
#include "Utils/ErrorHandling.hpp"
#include "VEngine/Core/Window.hpp"

ven::Window::~Window() {
    glfwTerminate();
}

GLFWwindow* ven::Window::createWindow(const uint16_t width, const uint16_t height, const std::string& title) {
    if (glfwInit() == GLFW_FALSE) {
        throw utl::THROW_ERROR("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw utl::THROW_ERROR("Failed to create window");
    }
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    return window;
}

void ven::Window::createWindowSurface(const VkInstance& instance, VkSurfaceKHR* surface) const {
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
        throw utl::THROW_ERROR("Failed to create window surface");
    }
}

void ven::Window::setWindowIcon(const std::string& path) const {
    static const utl::Image image(path);
    if (image.pixels == nullptr) {
        throw utl::THROW_ERROR("Failed to load window icon");
    }
    static const GLFWimage appIcon{ .width = image.width, .height = image.height, .pixels = image.pixels };
    glfwSetWindowIcon(m_window, 1, &appIcon);
}

void ven::Window::setFullscreen(const bool fullscreen, const uint16_t width, const uint16_t height) {
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

void ven::Window::pollEvents() { glfwPollEvents(); }
void ven::Window::waitEvents() { glfwWaitEvents(); }
void ven::Window::getFrameBufferSize(int& width, int& height) const { glfwGetFramebufferSize(m_window, &width, &height); }
bool ven::Window::shouldClose() const { return glfwWindowShouldClose(m_window) != 0; }
bool ven::Window::isKeyPressed(const int key) const { return glfwGetKey(m_window, key) == GLFW_PRESS; }
const char **ven::Window::getRequiredInstanceExtensions(uint32_t *count) { return glfwGetRequiredInstanceExtensions(count); }
void ven::Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height) { static_cast<Window *>(glfwGetWindowUserPointer(window))->m_frameBufferResized = true; }
VkExtent2D ven::Window::getExtent() const {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}
