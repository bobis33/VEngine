///
/// @file Camera.hpp
/// @brief This file contains the Camera class
/// @namespace ven
///

#pragma once

#include <glm/glm.hpp>

namespace ven {

    static constexpr glm::vec3 DEFAULT_POSITION{0.F, 0.F, -2.5F};
    static constexpr glm::vec3 DEFAULT_ROTATION{0.F, 0.F, 0.F};

    static constexpr float DEFAULT_FOV = glm::radians(50.0F);
    static constexpr float DEFAULT_NEAR = 0.1F;
    static constexpr float DEFAULT_FAR = 100.F;

    ///
    /// @class Camera
    /// @brief Class for camera
    /// @namespace ven
    ///
    class Camera {

        public:

            void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
            void setPerspectiveProjection(float aspect);
            void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.F, -1.F, 0.F});
            void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.F, -1.F, 0.F}) { setViewDirection(position, target - position, up); }
            void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
            void setFov(float fov) { m_fov = fov; }
            void setNear(float near) { m_near = near; }
            void setFar(float far) { m_far = far; }

            [[nodiscard]] const glm::mat4& getProjection() const { return m_projectionMatrix; }
            [[nodiscard]] const glm::mat4& getView() const { return m_viewMatrix; }
            [[nodiscard]] const glm::mat4& getInverseView() const { return m_inverseViewMatrix; }
            [[nodiscard]] float getFov() const { return m_fov; }
            [[nodiscard]] float getNear() const { return m_near; }
            [[nodiscard]] float getFar() const { return m_far; }

        private:

            float m_fov{DEFAULT_FOV};
            float m_near{DEFAULT_NEAR};
            float m_far{DEFAULT_FAR};
            glm::mat4 m_projectionMatrix{1.F};
            glm::mat4 m_viewMatrix{1.F};
            glm::mat4 m_inverseViewMatrix{1.F};

    }; // class Camera

} // namespace ven
