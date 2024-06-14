#include <stdexcept>

#include "VEngine/Window.hpp"

GLFWwindow* ven::Window::createWindow(const int width, const int height, const std::string &title)
{
    GLFWwindow *window = nullptr;
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    return window;
}

void ven::Window::destroyWindow()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
}
