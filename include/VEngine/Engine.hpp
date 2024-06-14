/*
** @file Engine.hpp
** @brief This file contains the Engine class
** @namespace ven
*/

#pragma once

#include <vulkan/vulkan.h>
#include <memory>

#include "VEngine/Window.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"

namespace ven {

    class Engine {

    public:
        explicit Engine(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT, const std::string &title = "VEngine") : m_window(width, height, title) {};
        ~Engine() = default;

        Window &getWindow() { return m_window; };

        void initVulkan();
        void mainLoop();
        void deleteResources();

    private:
        Window m_window;
        std::unique_ptr<Device> m_device{nullptr};
        VkInstance m_instance{nullptr};
        VkSurfaceKHR m_surface{nullptr};

        void createInstance();
        void createSurface();
    };

} // namespace ven
