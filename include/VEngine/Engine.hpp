///
/// @file Engine.hpp
/// @brief This file contains the Engine class
/// @namespace ven
///

#pragma once

#include <vulkan/vulkan.h>

#include "VEngine/Gui.hpp"
#include "VEngine/Window.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Renderer.hpp"
#include "VEngine/Descriptors/DescriptorPool.hpp"
#include "VEngine/Light.hpp"
#include "VEngine/SceneManager.hpp"

namespace ven {

    enum ENGINE_STATE : uint8_t {
        EDITOR = 0,
        GAME = 1,
        PAUSED = 2,
        EXIT = 3
    };

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

        private:

            void loadObjects();

            ENGINE_STATE m_state{EXIT};

            Window m_window;
            Device m_device{m_window};
            Renderer m_renderer{m_window, m_device};
            Gui m_gui;
            std::unique_ptr<DescriptorPool> m_globalPool;
            std::vector<std::unique_ptr<DescriptorPool>> framePools;
            SceneManager m_objectManager{m_device};
            Light::Map m_lights;

            VkInstance m_instance{nullptr};
            VkSurfaceKHR m_surface{nullptr};

            void createInstance();
            void createSurface() { if (glfwCreateWindowSurface(m_instance, m_window.getGLFWindow(), nullptr, &m_surface) != VK_SUCCESS) { throw std::runtime_error("Failed to create window surface"); } }

    }; // class Engine

} // namespace ven
