///
/// @file ImGuiWindowManager.hpp
/// @brief This file contains the ImGuiWindowManager class
/// @namespace ven
///

#pragma once

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui.h>
#include <imgui_internal.h>

#include "VEngine/Object.hpp"
#include "VEngine/Renderer.hpp"

namespace ven {

    class ImGuiWindowManager {

    public:

        ImGuiWindowManager() = default;
        ~ImGuiWindowManager() = default;

        ImGuiWindowManager(const ImGuiWindowManager&) = delete;
        ImGuiWindowManager& operator=(const ImGuiWindowManager&) = delete;

        static void initImGui(GLFWwindow* window, VkInstance instance, Device* device, VkRenderPass renderPass);
        void imGuiRender(Renderer *renderer, std::unordered_map<id_t, Object>& objects, ImGuiIO& io, Object& camera, VkPhysicalDevice physicalDevice);
        static void imGuiRenderDemo(Renderer* renderer);
        // void imGuiCleanup();

    };
}
