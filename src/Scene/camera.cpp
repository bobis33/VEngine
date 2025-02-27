#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include "VEngine/Scene/Camera.hpp"

void ven::Camera::move(const glm::vec3& direction, const float deltaTime) {
    const float distance = m_moveSpeed * deltaTime;
    m_position += direction * distance;
}

void ven::Camera::rotate(const float yawOffset, const float pitchOffset, const float deltaTime) {
    const float distance = m_lookSpeed * deltaTime;
    m_yaw += yawOffset * distance;
    m_pitch += pitchOffset * distance;
    m_pitch = glm::clamp(m_pitch, -89.0F, 89.0F);
    m_front = glm::normalize(glm::vec3(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)), sin(glm::radians(m_pitch)), sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))));
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 ven::Camera::getProjectionMatrix(const float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, m_near, m_far);
}

glm::mat4 ven::Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}
