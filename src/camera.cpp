#include <cassert>
#include <cmath>

#include "VEngine/Camera.hpp"

void ven::Camera::setOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near, const float far)
{
    m_projectionMatrix = glm::mat4{1.0F};
    m_projectionMatrix[0][0] = 2.F / (right - left);
    m_projectionMatrix[1][1] = 2.F / (bottom - top);
    m_projectionMatrix[2][2] = 1.F / (far - near);
    m_projectionMatrix[3][0] = -(right + left) / (right - left);
    m_projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
    m_projectionMatrix[3][2] = -near / (far - near);
}

void ven::Camera::setPerspectiveProjection(const float aspect)
{
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0F);
    const float tanHalfFov = std::tan(m_fov / 2.F);
    m_projectionMatrix = glm::mat4{0.0F};
    m_projectionMatrix[0][0] = 1.F / (aspect * tanHalfFov);
    m_projectionMatrix[1][1] = 1.F / (tanHalfFov);
    m_projectionMatrix[2][2] = m_far / (m_far - m_near);
    m_projectionMatrix[2][3] = 1.F;
    m_projectionMatrix[3][2] = -(m_far * m_near) / (m_far - m_near);
}

void ven::Camera::setViewDirection(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 up)
{
    const glm::vec3 w{normalize(direction)};
    const glm::vec3 u{normalize(cross(w, up))};
    const glm::vec3 v{cross(w, u)};

    m_viewMatrix = glm::mat4{1.F};
    m_viewMatrix[0][0] = u.x;
    m_viewMatrix[1][0] = u.y;
    m_viewMatrix[2][0] = u.z;
    m_viewMatrix[0][1] = v.x;
    m_viewMatrix[1][1] = v.y;
    m_viewMatrix[2][1] = v.z;
    m_viewMatrix[0][2] = w.x;
    m_viewMatrix[1][2] = w.y;
    m_viewMatrix[2][2] = w.z;
    m_viewMatrix[3][0] = -dot(u, position);
    m_viewMatrix[3][1] = -dot(v, position);
    m_viewMatrix[3][2] = -dot(w, position);

    m_inverseViewMatrix = glm::mat4{1.F};
    m_inverseViewMatrix[0][0] = u.x;
    m_inverseViewMatrix[0][1] = u.y;
    m_inverseViewMatrix[0][2] = u.z;
    m_inverseViewMatrix[1][0] = v.x;
    m_inverseViewMatrix[1][1] = v.y;
    m_inverseViewMatrix[1][2] = v.z;
    m_inverseViewMatrix[2][0] = w.x;
    m_inverseViewMatrix[2][1] = w.y;
    m_inverseViewMatrix[2][2] = w.z;
    m_inverseViewMatrix[3][0] = position.x;
    m_inverseViewMatrix[3][1] = position.y;
    m_inverseViewMatrix[3][2] = position.z;
}

void ven::Camera::setViewYXZ(const glm::vec3 position, const glm::vec3 rotation)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
    m_viewMatrix = glm::mat4{1.F};
    m_viewMatrix[0][0] = u.x;
    m_viewMatrix[1][0] = u.y;
    m_viewMatrix[2][0] = u.z;
    m_viewMatrix[0][1] = v.x;
    m_viewMatrix[1][1] = v.y;
    m_viewMatrix[2][1] = v.z;
    m_viewMatrix[0][2] = w.x;
    m_viewMatrix[1][2] = w.y;
    m_viewMatrix[2][2] = w.z;
    m_viewMatrix[3][0] = -dot(u, position);
    m_viewMatrix[3][1] = -dot(v, position);
    m_viewMatrix[3][2] = -dot(w, position);

    m_inverseViewMatrix = glm::mat4{1.F};
    m_inverseViewMatrix[0][0] = u.x;
    m_inverseViewMatrix[0][1] = u.y;
    m_inverseViewMatrix[0][2] = u.z;
    m_inverseViewMatrix[1][0] = v.x;
    m_inverseViewMatrix[1][1] = v.y;
    m_inverseViewMatrix[1][2] = v.z;
    m_inverseViewMatrix[2][0] = w.x;
    m_inverseViewMatrix[2][1] = w.y;
    m_inverseViewMatrix[2][2] = w.z;
    m_inverseViewMatrix[3][0] = position.x;
    m_inverseViewMatrix[3][1] = position.y;
    m_inverseViewMatrix[3][2] = position.z;
}
