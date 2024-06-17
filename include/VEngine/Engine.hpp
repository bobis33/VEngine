/*
** @file Engine.hpp
** @brief This file contains the Engine class
** @namespace ven
*/

#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "VEngine/Window.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Shaders.hpp"

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
        Device m_device{m_window};
        Shaders m_shaders{m_device, "shaders/bin/fragment.spv", "shaders/bin/vertex.spv", Shaders::defaultPipelineConfigInfo(DEFAULT_WIDTH, DEFAULT_HEIGHT)};
        VkInstance m_instance{nullptr};
        VkSurfaceKHR m_surface{nullptr};

        void createInstance();
        void createSurface();
    };

} // namespace ven
