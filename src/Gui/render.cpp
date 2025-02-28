#include <algorithm>
#include <fstream>
#include <numeric>

#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "VEngine/Gfx/Resources/Model.hpp"
#include "VEngine/Gfx/Resources/Texture.hpp"
#include "VEngine/Gfx/Resources/TextureManager.hpp"
#include "VEngine/Gui/Gui.hpp"

static bool IsLegacyNativeDupe(const ImGuiKey key) { return key >= 0 && key < 512; }

void frameSection(const float frameRate, const ven::FrameStats& frameStats, float& graphMaxFps) {
    const auto displayFrameTimes = frameStats.getDisplayFrameTimes();
    const auto fpsTimes = ven::FrameStats::calculateFPS(displayFrameTimes);
    const float upperBound = ven::FrameStats::calculateUpperBound(displayFrameTimes);
    const float avgFrameTime = std::accumulate(displayFrameTimes.begin(), displayFrameTimes.end(), 0.0F) / ven::FrameStats::MAX_FRAME_TIMES;
    const float minFrameTime = *std::ranges::min_element(displayFrameTimes);
    const float maxFrameTime = *std::ranges::max_element(displayFrameTimes);
    const float avgFPS = std::accumulate(fpsTimes.begin(), fpsTimes.end(), 0.0F) / ven::FrameStats::MAX_FRAME_TIMES;
    const float minFPS = *std::ranges::min_element(fpsTimes);
    const float maxFPS = *std::ranges::max_element(fpsTimes);
    if (ImGui::CollapsingHeader("Frame")) {
        ImGui::Spacing();
        ImGui::Text("FPS: %.1f", frameRate);
        ImGui::PlotHistogram(
            "##fps",
            fpsTimes.data(),
            ven::FrameStats::MAX_FRAME_TIMES,
            0,
            nullptr,
            0.0F,
            graphMaxFps,
            ImVec2(285, 60)
        );
        ImGui::SliderFloat("Max scale", &graphMaxFps, 0.0F, 10000.0F);
        ImGui::Text("Frame time: %.3f ms", 1000.0F / frameRate);
        ImGui::PlotHistogram(
                "##frame",
                displayFrameTimes.data(),
                ven::FrameStats::MAX_FRAME_TIMES,
                0,
                nullptr,
                0.0F,
                upperBound,
                ImVec2(285, 60)
         );
        if (ImGui::Button("Export Data to CSV")) {
            ven::FrameStats::exportDataToCSV(displayFrameTimes, fpsTimes, "frametimes_and_fps.csv");
        }
        ImGui::Columns(2, nullptr, false);
        ImGui::Text("Avg Frame Time: %.3f ms", avgFrameTime * 1000.0F);
        ImGui::NextColumn();
        ImGui::Text("Avg FPS: %.1f", avgFPS);
        ImGui::NextColumn();
        ImGui::Text("Min Frame Time: %.3f ms", minFrameTime * 1000.0F);
        ImGui::NextColumn();
        ImGui::Text("Min FPS: %.1f", minFPS);
        ImGui::NextColumn();
        ImGui::Text("Max Frame Time: %.3f ms", maxFrameTime * 1000.0F);
        ImGui::NextColumn();
        ImGui::Text("Max FPS: %.1f", maxFPS);
        ImGui::Columns(1);
        ImGui::Spacing();
    }
}

void memorySection(const ven::MemoryMonitor& memoryMonitor) {
    const double totalMemory = memoryMonitor.getTotalMemory();
    const double usedMemory = memoryMonitor.getMemoryUsage();
    const double availableMemory = memoryMonitor.getAvailableMemory();
    const double totalSwap = memoryMonitor.getTotalSwap();
    const double usedSwap = memoryMonitor.getSwapUsage();
    const double freeSwap = memoryMonitor.getFreeSwap();
    std::array<char, 64> memoryUsage{};
    std::array<char, 64> swapUsage{};
    snprintf(memoryUsage.data(), sizeof(memoryUsage), "Memory Usage: %.2f MB / %.2f MB", usedMemory, totalMemory);
    snprintf(swapUsage.data(), sizeof(swapUsage), "Swap Usage: %.2f MB / %.2f MB", usedSwap, totalSwap);
    if (ImGui::CollapsingHeader("Memory")) {
        ImGui::Spacing();
        ImGui::ProgressBar(usedMemory / totalMemory, ImVec2(280.0F, 0.0F), memoryUsage.data());
        ImGui::Text("Available: %.2f MB", availableMemory);
        ImGui::ProgressBar(usedSwap / totalSwap, ImVec2(280.0F, 0.0F), swapUsage.data());
        ImGui::Text("Available: %.2f MB", freeSwap);
        ImGui::Spacing();
    }
}

void rendererSection(std::array<VkClearValue, 2>& clearValues) {
    if (ImGui::CollapsingHeader("Renderer")) {
        ImGui::Spacing();
        ImGui::ColorEdit4("Clear Color", clearValues.at(0).color.float32);
        ImGui::SliderFloat("Depth", &clearValues.at(1).depthStencil.depth, 0.0F, 1.0F);
        int stencilValue = static_cast<int>(clearValues.at(1).depthStencil.stencil);
        if (ImGui::SliderInt("Stencil", &stencilValue, 0, 255)) {
            clearValues[1].depthStencil.stencil = static_cast<uint32_t>(stencilValue);
        }
        ImGui::Spacing();
    }
}

void lightSection(glm::vec3& ambientColor) {
    if (ImGui::CollapsingHeader("Light")) {
        ImGui::Spacing();
        ImGui::ColorEdit3("Ambient", glm::value_ptr(ambientColor));
        ImGui::Spacing();
    }
}

void objectsSection(std::vector<ven::Model>& models) {
    int count = 0;
    if (ImGui::CollapsingHeader("Objects")) {
        ImGui::Spacing();
        for (auto& model : models) {
            count++;
            const std::string number = std::to_string(count);
            ImGui::Separator();
            ImGui::Text("Object %d", count);
            auto&[position, rotation, scale] = model.getTransform();
            ImGui::SliderFloat3(("Position##" + number).c_str(), glm::value_ptr(position), -100.0F, 100.0F);
            ImGui::SliderFloat3(("Rotation##" +number).c_str(), glm::value_ptr(rotation), -180.0F, 180.0F);
            ImGui::SliderFloat3(("Scale##" + number).c_str(), glm::value_ptr(scale), 0.1F, 10.0F);
        }
        ImGui::Spacing();
    }
}

void cameraSection(ven::Camera& camera) {
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::Spacing();
        ImGui::InputFloat3("Position", glm::value_ptr(camera.getPosition()));
        ImGui::InputFloat3("Front", glm::value_ptr(camera.getFront()));
        ImGui::InputFloat3("Up", glm::value_ptr(camera.getUp()));
        ImGui::InputFloat3("Right", glm::value_ptr(camera.getRight()));
        ImGui::SliderFloat("FOV", &camera.getFov(), 1.0F, 180.0F);
        ImGui::SliderFloat("Near", &camera.getNear(), 0.1F, 10.0F);
        ImGui::SliderFloat("Far", &camera.getFar(), 10.0F, 1000.0F);
        ImGui::SliderFloat("Move Speed", &camera.getMoveSpeed(), 0.1F, 100.0F);
        ImGui::SliderFloat("Look Speed", &camera.getLookSpeed(), 1.0F, 1000.0F);
        ImGui::Spacing();
    }
}

void inputsSection(const ImGuiIO& imGui) {
    if (ImGui::CollapsingHeader("Inputs")) {
        ImGui::Spacing();
        ImGui::IsMousePosValid() ? ImGui::Text("Mouse pos: (%g, %g)", imGui.MousePos.x, imGui.MousePos.y) : ImGui::Text("Mouse pos: <INVALID>");
        ImGui::Text("Mouse delta: (%g, %g)", imGui.MouseDelta.x, imGui.MouseDelta.y);
        ImGui::Text("Mouse down:");
        for (int i = 0; i < static_cast<int>(std::size(imGui.MouseDown)); i++) {
            if (ImGui::IsMouseDown(i)) {
                ImGui::SameLine();
                ImGui::Text("b%d (%.02f secs)", i, imGui.MouseDownDuration[i]);
            }
        }
        ImGui::Text("Mouse wheel: %.1f", imGui.MouseWheel);
        ImGui::Text("Keys down:");
        for (auto key = static_cast<ImGuiKey>(0); key < ImGuiKey_NamedKey_END; key = static_cast<ImGuiKey>(key + 1)) {
            if (IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) { continue; }
            ImGui::SameLine();
            ImGui::Text(key < ImGuiKey_NamedKey_BEGIN ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        }
        ImGui::Spacing();
    }
}

void devicePropertiesSection(const VkPhysicalDeviceProperties& deviceProperties) {
    if (ImGui::BeginMenu("Device Properties")) {
        if (ImGui::BeginTable("DevicePropertiesTable", 2)) {
            ImGui::TableNextColumn(); ImGui::Text("Device Name: %s", deviceProperties.deviceName);
            ImGui::TableNextColumn(); ImGui::Text("API Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.apiVersion), VK_VERSION_MINOR(deviceProperties.apiVersion), VK_VERSION_PATCH(deviceProperties.apiVersion));
            ImGui::TableNextColumn(); ImGui::Text("Driver Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.driverVersion), VK_VERSION_MINOR(deviceProperties.driverVersion), VK_VERSION_PATCH(deviceProperties.driverVersion));
            ImGui::TableNextColumn(); ImGui::Text("Vendor ID: %d", deviceProperties.vendorID);
            ImGui::TableNextColumn(); ImGui::Text("Device ID: %d", deviceProperties.deviceID);
            ImGui::TableNextColumn(); ImGui::Text("Device Type: %d", deviceProperties.deviceType);
            ImGui::TableNextColumn(); ImGui::Text("Discrete Queue Priorities: %d", deviceProperties.limits.discreteQueuePriorities);
            ImGui::TableNextColumn(); ImGui::Text("Max Push Constants Size: %d", deviceProperties.limits.maxPushConstantsSize);
            ImGui::TableNextColumn(); ImGui::Text("Max Memory Allocation Count: %d", deviceProperties.limits.maxMemoryAllocationCount);
            ImGui::TableNextColumn(); ImGui::Text("Max Image Dimension 1D: %d", deviceProperties.limits.maxImageDimension1D);
            ImGui::TableNextColumn(); ImGui::Text("Max Image Dimension 2D: %d", deviceProperties.limits.maxImageDimension2D);
            ImGui::TableNextColumn(); ImGui::Text("Max Image Dimension 3D: %d", deviceProperties.limits.maxImageDimension3D);
            ImGui::TableNextColumn(); ImGui::Text("Max Image Dimension Cube: %d", deviceProperties.limits.maxImageDimensionCube);
            ImGui::TableNextColumn(); ImGui::Text("Max Image Array Layers: %d", deviceProperties.limits.maxImageArrayLayers);
            ImGui::TableNextColumn(); ImGui::Text("Max Texel Buffer Elements: %d", deviceProperties.limits.maxTexelBufferElements);
            ImGui::TableNextColumn(); ImGui::Text("Max Uniform Buffer Range: %d", deviceProperties.limits.maxUniformBufferRange);
            ImGui::TableNextColumn(); ImGui::Text("Max Storage Buffer Range: %d", deviceProperties.limits.maxStorageBufferRange);
            ImGui::EndTable();
        }
        ImGui::EndMenu();
    }
}

void themeSection() {
    static int selectedTheme = 0;
    static constexpr std::array themesNames = { "BlackWhite", "BlueGrey", "BlackRed" };
    if (ImGui::BeginMenu("Themes")) {
        for (int i = 0; i < themesNames.size(); ++i) {
            if (ImGui::MenuItem(themesNames.at(i), nullptr, selectedTheme == i)) {
                selectedTheme = i;
                switch (selectedTheme) {
                    case 0:
                        ven::Gui::applyTheme(ven::Gui::BlackWhite);
                    break;
                    case 1:
                        ven::Gui::applyTheme(ven::Gui::BlueGrey);
                    break;
                    case 2:
                        ven::Gui::applyTheme(ven::Gui::BlackRed);
                    break;
                    default:
                        ImGui::GetStyle() = ImGuiStyle();
                    break;
                }
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Reset Theme")) {
            ImGui::GetStyle() = ImGuiStyle();
            selectedTheme = -1;
        }
        ImGui::EndMenu();
    }
}

void ven::Gui::render(const VkCommandBuffer& commandBuffer) {
    const ImGuiIO& imGui = ImGui::GetIO();
    const float frameRate = imGui.Framerate;
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowMetricsWindow();
    ImGui::SetNextWindowPos(ImVec2(imGui.DisplaySize.x - RIGHT_PANEL_WIDTH, 19), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(RIGHT_PANEL_WIDTH, imGui.DisplaySize.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(RIGHT_PANEL_WIDTH, imGui.DisplaySize.y), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.2F, 0.2F, 0.2F, 0.1F));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2F, 0.2F, 0.2F, 0.5F));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2F, 0.2F, 0.2F, 0.5F));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2F, 0.2F, 0.2F, 0.5F));
    if (ImGui::Begin("##Right Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
        m_frameStats.updateFrameTimes(imGui.DeltaTime);
        frameSection(frameRate, m_frameStats, m_graphMaxFps);
        m_memoryMonitor.update();
        memorySection(m_memoryMonitor);
        rendererSection(m_clearValues);
        cameraSection(m_camera);
        lightSection(m_ambientColor);
        objectsSection(m_models);
        inputsSection(imGui);
    }
    ImGui::End();
    ImGui::BeginMainMenuBar();
    devicePropertiesSection(m_deviceProperties);
    themeSection();
    ImGui::EndMainMenuBar();
    ImGui::PopStyleColor(4);
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}
