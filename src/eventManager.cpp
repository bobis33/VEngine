#include "VEngine/EventManager.hpp"

void ven::EventManager::handleEvents(GLFWwindow *window, ENGINE_STATE *engineState)
{
    if (glfwWindowShouldClose(window) == GLFW_TRUE) {
        updateEngineState(engineState, EXIT);
    }
}
