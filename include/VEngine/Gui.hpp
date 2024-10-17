///
/// @file Gui.hpp
/// @brief This file contains the ImGuiWindowManager class
/// @namespace ven
///

#pragma once

#include <imgui.h>

#include "VEngine/Object.hpp"
#include "VEngine/Renderer.hpp"
#include "VEngine/Camera.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    static constexpr uint16_t DESCRIPTOR_COUNT = 1000;

    enum GUI_STATE : uint8_t {
        VISIBLE = 0,
        HIDDEN = 1
    };

    ///
    /// @class Gui
    /// @brief Class for Gui
    /// @namespace ven
    ///
    class Gui {

        public:

            Gui() = default;
            ~Gui() = default;

            Gui(const Gui&) = delete;
            Gui& operator=(const Gui&) = delete;

            static void init(GLFWwindow* window, VkInstance instance, const Device* device, VkRenderPass renderPass);

            static void render(Renderer *renderer, std::unordered_map<unsigned int, Object>& objects, std::unordered_map<unsigned int, Light>& lights, Camera& camera, VkPhysicalDevice physicalDevice, GlobalUbo& ubo);
            static void cleanup();

            void setState(const GUI_STATE state) { m_state = state; }
            [[nodiscard]] GUI_STATE getState() const { return m_state; }

        private:

            static void initStyle();
            static void renderFrameWindow();
            static void cameraSection(Camera& camera);
            static void inputsSection(const ImGuiIO *io);
            static void rendererSection(Renderer *renderer, GlobalUbo& ubo);
            static void devicePropertiesSection(VkPhysicalDeviceProperties deviceProperties);
            static void objectsSection(std::unordered_map<unsigned int, Object>& objects);
            static void lightsSection(std::unordered_map<unsigned int, Light>& lights);


            struct funcs { static bool IsLegacyNativeDupe(const ImGuiKey key) { return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exist

            static ImGuiIO* m_io;
            GUI_STATE m_state{VISIBLE};
            static float m_intensity;
            static float m_shininess;

    }; // class Gui

} // namespace ven
