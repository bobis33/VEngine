///
/// @file Gui.hpp
/// @brief This file contains the Gui class
/// @namespace ven
///

#pragma once

#include "Utils/FrameStats.hpp"
#include "Utils/MemoryMonitor.hpp"
#include "VEngine/Gfx/Backend/Device.hpp"
#include "VEngine/Gfx/Resources/Model.hpp"
#include "VEngine/Scene/Camera.hpp"

namespace ven {

    ///
    /// @class Gui
    /// @brief Class for Gui
    /// @namespace ven
    ///
    class Gui {

        public:

            enum Theme: uint8_t {
                BlackWhite = 0x00,
                BlueGrey = 0x01,
                BlackRed = 0x02
            };

            Gui(const Device& device, Camera& camera, GLFWwindow* window, const VkRenderPass& renderPass, std::vector<Model>& models, std::array<VkClearValue, 2>& clearValues, glm::vec3& ambientColor);
            ~Gui();

            Gui(const Gui&) = delete;
            Gui& operator=(const Gui&) = delete;
            Gui(Gui&&) = delete;
            Gui& operator=(Gui&&) = delete;

            void render(const VkCommandBuffer& commandBuffer);
            static void applyTheme(const Theme theme) { switch (theme) { case BlackRed: blackRedTheme(); break; case BlackWhite: blackWhiteTheme(); break; case BlueGrey: blueGreyTheme(); } }

        private:

            static constexpr float RIGHT_PANEL_WIDTH = 300.0F;
            static constexpr float GRAPH_MAX_FPS = 10000.0F;

            static void blackRedTheme();
            static void blackWhiteTheme();
            static void blueGreyTheme();

            const VkDevice& m_device;
            VkDescriptorPool m_pool = VK_NULL_HANDLE;
            VkPhysicalDeviceProperties m_deviceProperties{};
            std::array<VkClearValue, 2>& m_clearValues;
            glm::vec3& m_ambientColor;
            float m_graphMaxFps{GRAPH_MAX_FPS};
            Camera& m_camera;
            std::vector<Model>& m_models;
            FrameStats m_frameStats;
            MemoryMonitor m_memoryMonitor;

    }; // class Gui

} // namespace ven
