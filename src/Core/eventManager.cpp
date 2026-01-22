#include <array>
#include <string>

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

#include "VEngine/Core/EventManager.hpp"

constexpr float EPSILON = std::numeric_limits<float>::epsilon();

void ven::EventManager::handleEvents(const float dt) const {
    glm::vec3 moveDir(0.0F);
    float yawOffset = 0.0F;
    float pitchOffset = 0.0F;
    const std::array<std::pair<uint16_t, glm::vec3>, 6> moveMappings = {{
        {m_keyMappings.moveForward, m_camera.getFront()},
        {m_keyMappings.moveBackward, -m_camera.getFront()},
        {m_keyMappings.moveLeft, -m_camera.getRight()},
        {m_keyMappings.moveRight, m_camera.getRight()},
        {m_keyMappings.moveUp, m_camera.getUp()},
        {m_keyMappings.moveDown, -m_camera.getUp()}
    }};
    const std::array<std::pair<uint16_t, float*>, 4> lookMappings = {{
        {m_keyMappings.lookLeft, &yawOffset},
        {m_keyMappings.lookRight, &yawOffset},
        {m_keyMappings.lookUp, &pitchOffset},
        {m_keyMappings.lookDown, &pitchOffset}
    }};
    Window::pollEvents();

    for (const auto& [key, direction] : moveMappings) {
        if (m_window.isKeyPressed(key)) {
            moveDir += direction;
        }
    }
    if (glm::length(moveDir) > EPSILON) {
        m_camera.move(moveDir, dt);
    }
    for (const auto& [key, offset] : lookMappings) {
        if (m_window.isKeyPressed(key)) {
            *offset += key == m_keyMappings.lookLeft || key == m_keyMappings.lookDown ? -1.0F : 1.0F;
        }
    }
    if (yawOffset != 0.0F || pitchOffset != 0.0F) {
        m_camera.rotate(yawOffset, pitchOffset, dt);
    }
}
