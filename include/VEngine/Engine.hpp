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
#include "VEngine/Shaders.hpp"
#include "VEngine/SwapChain.hpp"
#include "VEngine/Model.hpp"

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

        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void freeCommandBuffers();

        Window m_window;
        Device m_device{m_window};
        std::unique_ptr<SwapChain> m_swapChain;
        std::unique_ptr<Shaders> m_shaders;
        std::unique_ptr<Model> m_model;

        VkPipelineLayout m_pipelineLayout{nullptr};
        std::vector<VkCommandBuffer> m_commandBuffers;

        VkInstance m_instance{nullptr};
        VkSurfaceKHR m_surface{nullptr};

        void createInstance();
        void createSurface();
    }; // class Engine

} // namespace ven
