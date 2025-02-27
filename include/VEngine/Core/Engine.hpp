///
/// @file Engine.hpp
/// @brief This file contains the Engine class
/// @namespace ven
///

#pragma once

#include "Utils/Clock.hpp"
#include "VEngine/Core/EventManager.hpp"
#include "VEngine/Gfx/Backend/Descriptors/Pool.hpp"
#include "VEngine/Gfx/Backend/Descriptors/SetLayout.hpp"
#include "VEngine/Gfx/Backend/Descriptors/Sets.hpp"
#include "VEngine/Gfx/Renderer.hpp"
#include "VEngine/Gfx/Resources/TextureManager.hpp"

namespace ven {

    ///
    /// @class Engine
    /// @brief Class for engine
    /// @namespace ven
    ///
    class Engine {

        public:

            Engine(): m_device(m_window), m_descriptorPool(m_device.getVkDevice()), m_descriptorSetLayout(m_device.getVkDevice()),
                      m_descriptorSets(m_device.getVkDevice(), m_descriptorPool.getDescriptorPool(), m_descriptorSetLayout.getDescriptorSetLayout(),m_uniformBuffers),
                      m_renderer(m_device, m_window, m_models), m_eventManager(m_renderer.getCamera(), m_window) { loadAssets(); init(); }

            ~Engine() {
                const VkDevice& device = m_device.getVkDevice();
                TextureManager::clean();
                for (uint8_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
                    vkDestroyBuffer(device, m_uniformBuffers.at(i), nullptr);
                    vkFreeMemory(device, m_uniformBuffersMemory.at(i), nullptr);
                }
                vkDestroyBuffer(device, m_indexBuffer, nullptr);
                vkFreeMemory(device, m_indexBufferMemory, nullptr);
                vkDestroyBuffer(device, m_vertexBuffer, nullptr);
                vkFreeMemory(device, m_vertexBufferMemory, nullptr);
            }

            Engine(const Engine &) = delete;
            Engine &operator=(const Engine &) = delete;
            Engine(Engine &&) = delete;
            Engine &operator=(Engine &&) = delete;

            void run();

        private:

            void init();
            void loadAssets();
            void drawFrame();
            void createUniformBuffers();

            uint32_t m_indicesSize = 0;
            Window m_window;
            Device m_device;
            DescriptorPool m_descriptorPool;
            DescriptorSetLayout m_descriptorSetLayout;
            DescriptorSets m_descriptorSets;
            Renderer m_renderer;
            EventManager m_eventManager;
            utl::Clock m_clock;
            std::vector<Model> m_models;
            std::vector<VkBuffer> m_uniformBuffers;
            std::vector<VkDeviceMemory> m_uniformBuffersMemory;
            std::vector<void*> m_uniformBuffersMapped;
            std::vector<VkCommandBuffer> m_commandBuffers;
            VkBuffer m_vertexBuffer = nullptr;
            VkDeviceMemory m_vertexBufferMemory = nullptr;
            VkBuffer m_indexBuffer = nullptr;
            VkDeviceMemory m_indexBufferMemory = nullptr;
            uint32_t m_currentFrame = 0;

    }; // class Engine

} // namespace ven
