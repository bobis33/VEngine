///
/// @file Camera.hpp
/// @brief This file contains the Camera class
/// @namespace ven
///

#pragma once

#include "VEngine/Scene/Transform3D.hpp"

namespace ven {

    static constexpr glm::vec3 DEFAULT_POSITION{0.F, 0.F, -2.5F};
    static constexpr glm::vec3 DEFAULT_ROTATION{0.F, 0.F, 0.F};

    static constexpr float DEFAULT_FOV = glm::radians(50.0F);
    static constexpr float DEFAULT_NEAR = 0.1F;
    static constexpr float DEFAULT_FAR = 100.F;

    static constexpr float DEFAULT_MOVE_SPEED = 3.F;
    static constexpr float DEFAULT_LOOK_SPEED = 1.5F;

    ///
    /// @class Camera
    /// @brief Class for camera
    /// @namespace ven
    ///
    class Camera {

        public:

            Camera(const float fov, const float near, const float far, const float moveSpeed, const float lookSpeed) : m_fov(fov), m_near(near), m_far(far), m_moveSpeed(moveSpeed), m_lookSpeed(lookSpeed) { }
            ~Camera() = default;

            Camera(const Camera&) = delete;
            Camera& operator=(const Camera&) = delete;
            Camera(Camera&&) = default;
            Camera& operator=(Camera&&) = default;

            void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
            void setPerspectiveProjection(float aspect);
            void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = {0.F, -1.F, 0.F});
            void setViewTarget(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up = {0.F, -1.F, 0.F}) { setViewDirection(position, target - position, up); }
            void setViewXYZ(glm::vec3 position, glm::vec3 rotation);
            void setFov(const float fov) { m_fov = fov; }
            void setNear(const float near) { m_near = near; }
            void setFar(const float far) { m_far = far; }
            void setMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }
            void setLookSpeed(const float lookSpeed) { m_lookSpeed = lookSpeed; }

            [[nodiscard]] const glm::mat4& getProjection() const { return m_projectionMatrix; }
            [[nodiscard]] const glm::mat4& getView() const { return m_viewMatrix; }
            [[nodiscard]] const glm::mat4& getInverseView() const { return m_inverseViewMatrix; }
            [[nodiscard]] float getFov() const { return m_fov; }
            [[nodiscard]] float getNear() const { return m_near; }
            [[nodiscard]] float getFar() const { return m_far; }
            [[nodiscard]] float getMoveSpeed() const { return m_moveSpeed; }
            [[nodiscard]] float getLookSpeed() const { return m_lookSpeed; }

            Transform3D transform{DEFAULT_POSITION, {1.F, 1.F, 1.F}, DEFAULT_ROTATION};

        private:

            float m_fov;
            float m_near;
            float m_far;
            float m_moveSpeed;
            float m_lookSpeed;
            glm::mat4 m_projectionMatrix{1.F};
            glm::mat4 m_viewMatrix{1.F};
            glm::mat4 m_inverseViewMatrix{1.F};

    }; // class Camera

} // namespace ven
