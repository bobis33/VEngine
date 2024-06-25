/*
** @file Camera.hpp
** @brief This file contains the Camera class
** @namespace ven
*/

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace ven {

    class Camera {

        public:

            void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

            void setPerspectiveProjection(float fovy, float aspect, float near, float far);

            void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.F, -1.F, 0.F});
            void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.F, -1.F, 0.F});
            void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

            [[nodiscard]] const glm::mat4& getProjection() const { return m_projectionMatrix; };
            [[nodiscard]] const glm::mat4& getView() const { return m_viewMatrix; };

        private:

            glm::mat4 m_projectionMatrix{1.F};
            glm::mat4 m_viewMatrix{1.F};

    }; // class Camera

} // namespace ven
