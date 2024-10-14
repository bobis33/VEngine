///
/// @file Camera.hpp
/// @brief This file contains the KeyboardController class
/// @namespace ven
///

#pragma once

#include "VEngine/Window.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/Gui.hpp"

namespace ven {

    ///
    /// @class KeyboardController
    /// @brief Class for keyboard controller
    /// @namespace ven
    ///
    class KeyboardController {

        public:

             struct KeyMappings {
                uint16_t moveLeft = GLFW_KEY_A;
                uint16_t moveRight = GLFW_KEY_D;
                uint16_t moveForward = GLFW_KEY_W;
                uint16_t moveBackward = GLFW_KEY_S;
                uint16_t moveUp = GLFW_KEY_SPACE;
                uint16_t moveDown = GLFW_KEY_LEFT_SHIFT;
                uint16_t lookLeft = GLFW_KEY_LEFT;
                uint16_t lookRight = GLFW_KEY_RIGHT;
                uint16_t lookUp = GLFW_KEY_UP;
                uint16_t lookDown = GLFW_KEY_DOWN;
                uint16_t toggleGui = GLFW_KEY_F1;
            };

            void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object, Gui& gui, float moveSpeed, float lookSpeed) const;

            KeyMappings m_keys{};

        private:

             static bool isKeyJustPressed(GLFWwindow* window, int key, bool& wasPressed);

            mutable std::unordered_map<int, bool> m_keyStates;

    }; // class KeyboardController

} // namespace ven
