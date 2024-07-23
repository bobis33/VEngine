///
/// @file Engine.hpp
/// @brief This file contains the Engine class
/// @namespace ven
///

#pragma once

#include <vulkan/vulkan.h>

#include "VEngine/Window.hpp"
#include "VEngine/Constant.hpp"
#include "VEngine/Device.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/Renderer.hpp"
#include "VEngine/Descriptors.hpp"

namespace ven {

    class Engine {

    public:

        explicit Engine(uint32_t = DEFAULT_WIDTH, uint32_t = DEFAULT_HEIGHT, const std::string &title = DEFAULT_TITLE.data());
        ~Engine() = default;

        Engine(const Engine &) = delete;
        Engine operator=(const Engine &) = delete;

        Window &getWindow() { return m_window; };

        void mainLoop();

    private:

        void loadObjects();

        Window m_window;
        Device m_device{m_window};
        Renderer m_renderer{m_window, m_device};

        std::unique_ptr<DescriptorPool> m_globalPool;
        Object::Map m_objects;

        VkInstance m_instance{nullptr};
        VkSurfaceKHR m_surface{nullptr};

        void createInstance();
        void createSurface() { if (glfwCreateWindowSurface(m_instance, m_window.getGLFWindow(), nullptr, &m_surface) != VK_SUCCESS) { throw std::runtime_error("Failed to create window surface"); } }

    }; // class Engine

} // namespace ven
