///
/// @file EventManager.hpp
/// @brief This file contains the EventManager class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/Window.hpp"
#include "VEngine/Scene/Camera.hpp"

namespace ven {

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

            struct KeyMappings {
                uint16_t moveLeft;
                uint16_t moveRight;
                uint16_t moveForward;
                uint16_t moveBackward;
                uint16_t moveUp;
                uint16_t moveDown;
                uint16_t lookLeft;
                uint16_t lookRight;
                uint16_t lookUp;
                uint16_t lookDown;
                uint16_t toggleGui;
            };

            Camera &m_camera;
            const Window& m_window;
            static constexpr KeyMappings m_keyMappings = { .moveLeft = 65, .moveRight = 68, .moveForward = 87, .moveBackward = 83, .moveUp = 32, .moveDown = 340, .lookLeft = 263, .lookRight = 262, .lookUp = 265, .lookDown = 264, .toggleGui = 48 };

    }; // class EventManager

} // namespace ven
