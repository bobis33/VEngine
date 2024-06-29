/*
** @file Camera.hpp
** @brief This file contains the KeyboardController class
** @namespace ven
*/

#pragma once

#include "VEngine/Window.hpp"
#include "VEngine/Object.hpp"

namespace ven {

    class KeyboardController {

        public:

            struct KeyMappings {
                int moveLeft = GLFW_KEY_A;
                int moveRight = GLFW_KEY_D;
                int moveForward = GLFW_KEY_W;
                int moveBackward = GLFW_KEY_S;
                int moveUp = GLFW_KEY_SPACE;
                int moveDown = GLFW_KEY_LEFT_SHIFT;
                int lookLeft = GLFW_KEY_LEFT;
                int lookRight = GLFW_KEY_RIGHT;
                int lookUp = GLFW_KEY_UP;
                int lookDown = GLFW_KEY_DOWN;
            };

            void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object) const;

            KeyMappings m_keys{};
            float m_moveSpeed{3.F};
            float m_lookSpeed{1.5F};

    }; // class KeyboardController

} // namespace ven
