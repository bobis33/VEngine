///
/// @file EventManager.hpp
/// @brief This file contains the EventManager class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/Window.hpp"
#include "VEngine/Scene/Camera.hpp"

namespace ven {


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
        uint16_t toggleGui = GLFW_KEY_0;
    };

    static constexpr float EPSILON = std::numeric_limits<float>::epsilon();
    static constexpr KeyMappings DEFAULT_KEY_MAPPINGS{};

    ///
    /// @class EventManager
    /// @brief Class for event manager
    /// @namespace ven
    ///
    class EventManager {

        public:

            explicit EventManager(Camera& camera, const Window& window): m_camera(camera), m_window(window) { }
            ~EventManager() = default;

            EventManager(const EventManager&) = delete;
            EventManager& operator=(const EventManager&) = delete;
            EventManager(EventManager&&) = delete;
            EventManager& operator=(EventManager&&) = delete;

            void handleEvents(float dt) const;

        private:

            template<typename Iterator>
            static void processKeyActions(GLFWwindow* window, Iterator begin, Iterator end);
            static bool isKeyJustPressed(GLFWwindow* window, long unsigned int key, std::array<bool, GLFW_KEY_LAST>& keyStates);

            Camera &m_camera;
            const Window& m_window;

    }; // class EventManager

} // namespace ven
