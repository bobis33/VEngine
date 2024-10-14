///
/// @file EventManager.hpp
/// @brief This file contains the EventManager class
/// @namespace ven
///

#pragma once

#include "VEngine/Engine.hpp"

namespace ven {

    ///
    /// @class EventManager
    /// @brief Class for event manager
    /// @namespace ven
    ///
    class EventManager {

        public:

            EventManager() = default;
            ~EventManager() = default;

            EventManager(const EventManager&) = delete;
            EventManager& operator=(const EventManager&) = delete;

            static void handleEvents(GLFWwindow *window, ENGINE_STATE *engineState);

        private:

            static void updateEngineState(ENGINE_STATE *engineState, const ENGINE_STATE newState) { *engineState = newState; }

    }; // class EventManager

} // namespace ven