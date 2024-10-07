///
/// @file Camera.hpp
/// @brief This file contains the KeyboardController class
/// @namespace ven
///

#pragma once

#include "VEngine/Window.hpp"
#include "VEngine/Object.hpp"
#include "Light.hpp"

namespace ven {

    static constexpr float DEFAULT_MOVE_SPEED = 3.F;
    static constexpr float DEFAULT_LOOK_SPEED = 1.5F;

    ///
    /// @class KeyboardController
    /// @brief Class for keyboard controller
    /// @namespace ven
    ///
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

            void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object, bool* showDebugWindow) const;
            void moveInPlaneXZ(GLFWwindow* window, float dt, Light& object, bool* showDebugWindow) const;

            KeyMappings m_keys{};
            float m_moveSpeed{DEFAULT_MOVE_SPEED};
            float m_lookSpeed{DEFAULT_LOOK_SPEED};

    }; // class KeyboardController

} // namespace ven
