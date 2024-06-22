/*
** @file Engine.hpp
** @brief This file contains the Engine class
** @namespace ven
*/

#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "VEngine/Window.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/Renderer.hpp"

namespace ven {

    class Engine {

    public:

        explicit Engine(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT, const std::string &title = "VEngine");
        ~Engine();

        Engine(const Engine &) = delete;
        Engine operator=(const Engine &) = delete;

        Window &getWindow() { return m_window; };

        void mainLoop();

    private:

        void loadObjects();

        Window m_window;
        Device m_device{m_window};
        Renderer m_renderer{m_window, m_device};

        std::vector<Object> m_objects;

        VkInstance m_instance{nullptr};
        VkSurfaceKHR m_surface{nullptr};

        void createInstance();
        void createSurface();
    }; // class Engine

} // namespace ven
