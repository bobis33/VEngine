///
/// @file Engine.hpp
/// @brief This file contains the Engine class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/Gui.hpp"
#include "VEngine/Gfx/Renderer.hpp"
#include "VEngine/Gfx/Descriptors/Pool.hpp"
#include "VEngine/Scene/Manager.hpp"
#include "VEngine/Utils/Utils.hpp"

namespace ven {


    ///
    /// @class Engine
    /// @brief Class for engine
    /// @namespace ven
    ///
    class Engine {

        public:

            explicit Engine(uint32_t = DEFAULT_WIDTH, uint32_t = DEFAULT_HEIGHT, const std::string &title = DEFAULT_TITLE.data());
            ~Engine() = default;

            Engine(const Engine&) = delete;
            Engine operator=(const Engine&) = delete;

            void mainLoop();

            void cleanup();

        private:

            void loadObjects();

            ENGINE_STATE m_state{EXIT};

            Window m_window;
            Device m_device{m_window};
            Renderer m_renderer{m_window, m_device};
            Gui m_gui;
            std::unique_ptr<DescriptorPool> m_globalPool;
            std::vector<std::unique_ptr<DescriptorPool>> m_framePools;
            SceneManager m_sceneManager{m_device};

    }; // class Engine

} // namespace ven
