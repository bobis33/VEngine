#include "VEngine/Core/EventManager.hpp"

void ven::EventManager::handleEvents(float dt) const {
    Window::pollEvents();
    glm::vec3 moveDir(0.0F);
    const std::unordered_map<int, glm::vec3> moveDirections = {
        {DEFAULT_KEY_MAPPINGS.moveForward, m_camera.getFront()},
        {DEFAULT_KEY_MAPPINGS.moveBackward, -m_camera.getFront()},
        {DEFAULT_KEY_MAPPINGS.moveLeft, -m_camera.getRight()},
        {DEFAULT_KEY_MAPPINGS.moveRight, m_camera.getRight()},
        {DEFAULT_KEY_MAPPINGS.moveUp, m_camera.getUp()},
        {DEFAULT_KEY_MAPPINGS.moveDown, -m_camera.getUp()}
    };
    for (const auto& [key, direction] : moveDirections) {
        if (glfwGetKey(m_window.getGLFWWindow(), key) == GLFW_PRESS) {
            moveDir += direction;
        }
    }
    if (glm::length(moveDir) > EPSILON) {
        m_camera.move(glm::normalize(moveDir), dt);
    }
    float yawOffset = 0.0F, pitchOffset = 0.0F;
    const std::unordered_map<int, float*> lookOffsets = {
        {DEFAULT_KEY_MAPPINGS.lookLeft, &yawOffset},
        {DEFAULT_KEY_MAPPINGS.lookRight, &yawOffset},
        {DEFAULT_KEY_MAPPINGS.lookUp, &pitchOffset},
        {DEFAULT_KEY_MAPPINGS.lookDown, &pitchOffset}
    };
    for (const auto& [key, offset] : lookOffsets) {
        if (glfwGetKey(m_window.getGLFWWindow(), key) == GLFW_PRESS) {
            *offset += (key == DEFAULT_KEY_MAPPINGS.lookLeft || key == DEFAULT_KEY_MAPPINGS.lookDown) ? -1.0F : 1.0F;
        }
    }
    if (yawOffset != 0.0F || pitchOffset != 0.0F) {
        m_camera.rotate(yawOffset, pitchOffset, dt);
    }
}

bool ven::EventManager::isKeyJustPressed(GLFWwindow* window, const long unsigned int key, std::array<bool, GLFW_KEY_LAST>& keyStates) {
    const bool isPressed = glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    const bool wasPressed = keyStates.at(key);
    keyStates.at(key) = isPressed;
    return isPressed && !wasPressed;
}

template<typename Iterator>
void ven::EventManager::processKeyActions(GLFWwindow* window, Iterator begin, Iterator end) {
    for (auto it = begin; it != end; ++it) {
        if (glfwGetKey(window, it->key) == GLFW_PRESS) {
            *it->dir += it->value;
        }
    }
}
