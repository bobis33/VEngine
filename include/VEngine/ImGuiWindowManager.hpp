///
/// @file ImGuiWindowManager.hpp
/// @brief This file contains the ImGuiWindowManager class
/// @namespace ven
///

#pragma once

#include <imgui.h>

#include "VEngine/Object.hpp"
#include "VEngine/Renderer.hpp"
#include "VEngine/Camera.hpp"
#include "VEngine/KeyboardController.hpp"
#include "VEngine/FrameInfo.hpp"

namespace ven {

    ///
    /// @class ImGuiWindowManager
    /// @brief Class for ImGui window manager
    /// @namespace ven
    ///
    class ImGuiWindowManager {

        public:

            ImGuiWindowManager() = default;
            ~ImGuiWindowManager() = default;

            ImGuiWindowManager(const ImGuiWindowManager&) = delete;
            ImGuiWindowManager& operator=(const ImGuiWindowManager&) = delete;

            static void init(GLFWwindow* window, VkInstance instance, const Device* device, VkRenderPass renderPass);
            static void render(Renderer *renderer, std::unordered_map<unsigned int, Object>& objects, std::unordered_map<unsigned int, Light>& lights, Object& cameraObj, Camera& camera, KeyboardController& cameraController, VkPhysicalDevice physicalDevice, GlobalUbo& ubo);
            static void cleanup();

        private:

            static void initStyle();
            static void renderFrameWindow();
            static void cameraSection(Object& cameraObj, Camera& camera, KeyboardController& cameraController);
            static void inputsSection();
            static void rendererSection(Renderer *renderer, GlobalUbo& ubo);
            static void devicePropertiesSection(VkPhysicalDeviceProperties deviceProperties);
            static void objectsSection(std::unordered_map<unsigned int, Object>& objects);
            static void lightsSection(std::unordered_map<unsigned int, Light>& lights);

            struct funcs { static bool IsLegacyNativeDupe(const ImGuiKey key) { return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exist

            static ImGuiIO* m_io;

    }; // class ImGuiWindowManager

} // namespace ven
