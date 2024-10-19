///
/// @file Gui.hpp
/// @brief This file contains the ImGuiWindowManager class
/// @namespace ven
///

#pragma once

#include <imgui.h>

#include "VEngine/SceneManager.hpp"
#include "VEngine/Renderer.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    static constexpr uint16_t DESCRIPTOR_COUNT = 1000;

    enum GUI_STATE : uint8_t {
        SHOW_EDITOR = 0,
        SHOW_PLAYER = 1,
        HIDDEN = 2
    };

    ///
    /// @class Gui
    /// @brief Class for Gui
    /// @namespace ven
    ///
    class Gui {

        struct ClockData {
            float deltaTimeMS{0.0F};
            float fps{0.0F};
        };

        public:

            Gui() = default;
            ~Gui() = default;

            Gui(const Gui&) = delete;
            Gui& operator=(const Gui&) = delete;

            void init(GLFWwindow* window, VkInstance instance, const Device* device, VkRenderPass renderPass);

            void render(Renderer *renderer, SceneManager& sceneManager, Camera& camera, VkPhysicalDevice physicalDevice, GlobalUbo& ubo, const ClockData& clockData);
            static void cleanup();

            void setState(const GUI_STATE state) { m_state = state; }
            [[nodiscard]] GUI_STATE getState() const { return m_state; }
            [[nodiscard]] std::vector<unsigned int> *getObjectsToRemove() { return &m_objectsToRemove; }
            [[nodiscard]] std::vector<unsigned int> *getLightsToRemove() { return &m_lightsToRemove; }

        private:

            static void initStyle();
            static void renderFrameWindow(const ClockData& clockData);
            static void cameraSection(Camera& camera);
            static void inputsSection(const ImGuiIO& io);
            static void rendererSection(Renderer *renderer, GlobalUbo& ubo);
            static void devicePropertiesSection(VkPhysicalDeviceProperties deviceProperties);
            void objectsSection(SceneManager& sceneManager);
            void lightsSection(SceneManager& sceneManager);

            struct funcs { static bool IsLegacyNativeDupe(const ImGuiKey key) { return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exist

            ImGuiIO* m_io{nullptr};
            GUI_STATE m_state{HIDDEN};
            float m_intensity{1.0F};
            float m_shininess{DEFAULT_SHININESS};

            std::vector<unsigned int> m_objectsToRemove;
            std::vector<unsigned int> m_lightsToRemove;

    }; // class Gui

} // namespace ven
