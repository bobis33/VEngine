///
/// @file Engine.hpp
/// @brief This file contains the Engine class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/Gui.hpp"
#include "VEngine/Core/RenderSystem/Object.hpp"
#include "VEngine/Gfx/Renderer.hpp"
#include "VEngine/Gfx/Descriptors/Pool.hpp"
#include "VEngine/Gfx/Descriptors/SetLayout.hpp"
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
            Engine(Engine&&) = delete;
            Engine operator=(Engine&&) = delete;

            void run();

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

            std::unique_ptr<DescriptorSetLayout> m_globalSetLayout{DescriptorSetLayout::Builder(m_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT).addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT).build()};
            ObjectRenderSystem m_objectRenderSystem{m_device, m_renderer.getSwapChainRenderPass(), m_globalSetLayout->getDescriptorSetLayout()};
    }; // class Engine

} // namespace ven
