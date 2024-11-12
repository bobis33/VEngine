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
#include "VEngine/Utils/Config.hpp"

namespace ven {

    ///
    /// @class Engine
    /// @brief Class for engine
    /// @namespace ven
    ///
    class Engine {

        public:

            explicit Engine(const Config& config);
            ~Engine() { Gui::cleanup(); }

            Engine(const Engine&) = delete;
            Engine operator=(const Engine&) = delete;

            void mainLoop();

        private:

            void loadObjects();

            ENGINE_STATE m_state{EXIT};

            Window m_window;
            Camera m_camera;
            Gui m_gui;
            Device m_device{m_window};
            SceneManager m_sceneManager{m_device};
            Renderer m_renderer{m_window, m_device};
            std::unique_ptr<DescriptorPool> m_globalPool;
            std::vector<std::unique_ptr<DescriptorPool>> m_framePools;

    }; // class Engine

} // namespace ven
