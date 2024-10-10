#include <cmath>

#include "VEngine/KeyboardController.hpp"

void ven::KeyboardController::moveInPlaneXZ(GLFWwindow* window, const float dt, Object& object, bool* showDebugWindow) const
{
    glm::vec3 rotate{0};
    if (glfwGetKey(window, m_keys.lookLeft) == GLFW_PRESS) { rotate.y -= 1.F; }
    if (glfwGetKey(window, m_keys.lookRight) == GLFW_PRESS) { rotate.y += 1.F; }
    if (glfwGetKey(window, m_keys.lookUp) == GLFW_PRESS) { rotate.x += 1.F; }
    if (glfwGetKey(window, m_keys.lookDown) == GLFW_PRESS) { rotate.x -= 1.F; }

    if (dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        object.transform3D.rotation += m_lookSpeed * dt * normalize(rotate);
    }

    object.transform3D.rotation.x = glm::clamp(object.transform3D.rotation.x, -1.5F, 1.5F);
    object.transform3D.rotation.y = glm::mod(object.transform3D.rotation.y, glm::two_pi<float>());

    float yaw = object.transform3D.rotation.y;
    const glm::vec3 forwardDir{std::sin(yaw), 0.F, std::cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.F, -forwardDir.x};
    constexpr  glm::vec3 upDir{0.F, -1.F, 0.F};

    glm::vec3 moveDir{0.F};
    if (glfwGetKey(window, m_keys.moveForward) == GLFW_PRESS) {moveDir += forwardDir;}
    if (glfwGetKey(window, m_keys.moveBackward) == GLFW_PRESS) {moveDir -= forwardDir;}
    if (glfwGetKey(window, m_keys.moveRight) == GLFW_PRESS) {moveDir += rightDir;}
    if (glfwGetKey(window, m_keys.moveLeft) == GLFW_PRESS) {moveDir -= rightDir;}
    if (glfwGetKey(window, m_keys.moveUp) == GLFW_PRESS) {moveDir += upDir;}
    if (glfwGetKey(window, m_keys.moveDown) == GLFW_PRESS) {moveDir -= upDir;}

    if (dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        object.transform3D.translation += m_moveSpeed * dt * normalize(moveDir);
    }

    // imgui debug window
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        *showDebugWindow = !*showDebugWindow;
    }
}

void ven::KeyboardController::moveInPlaneXZ(GLFWwindow *window, const float dt, Light &light, bool *showDebugWindow) const
{
    glm::vec3 rotate{0};
    if (glfwGetKey(window, m_keys.lookLeft) == GLFW_PRESS) { rotate.y -= 1.F; }
    if (glfwGetKey(window, m_keys.lookRight) == GLFW_PRESS) { rotate.y += 1.F; }
    if (glfwGetKey(window, m_keys.lookUp) == GLFW_PRESS) { rotate.x += 1.F; }
    if (glfwGetKey(window, m_keys.lookDown) == GLFW_PRESS) { rotate.x -= 1.F; }

    if (dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        light.transform3D.rotation += m_lookSpeed * dt * normalize(rotate);
    }

    light.transform3D.rotation.x = glm::clamp(light.transform3D.rotation.x, -1.5F, 1.5F);
    light.transform3D.rotation.y = glm::mod(light.transform3D.rotation.y, glm::two_pi<float>());

    float yaw = light.transform3D.rotation.y;
    const glm::vec3 forwardDir{std::sin(yaw), 0.F, std::cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.F, -forwardDir.x};
    constexpr  glm::vec3 upDir{0.F, -1.F, 0.F};

    glm::vec3 moveDir{0.F};
    if (glfwGetKey(window, m_keys.moveForward) == GLFW_PRESS) {moveDir += forwardDir;}
    if (glfwGetKey(window, m_keys.moveBackward) == GLFW_PRESS) {moveDir -= forwardDir;}
    if (glfwGetKey(window, m_keys.moveRight) == GLFW_PRESS) {moveDir += rightDir;}
    if (glfwGetKey(window, m_keys.moveLeft) == GLFW_PRESS) {moveDir -= rightDir;}
    if (glfwGetKey(window, m_keys.moveUp) == GLFW_PRESS) {moveDir += upDir;}
    if (glfwGetKey(window, m_keys.moveDown) == GLFW_PRESS) {moveDir -= upDir;}

    if (dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        light.transform3D.translation += m_moveSpeed * dt * normalize(moveDir);
    }
}
