#include "Utils/Image.hpp"
#include "VEngine/Core/Window.hpp"

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
