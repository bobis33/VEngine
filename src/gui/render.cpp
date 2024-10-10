#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <glm/gtc/type_ptr.hpp>

#include "VEngine/ImGuiWindowManager.hpp"
#include "VEngine/Colors.hpp"


void ven::ImGuiWindowManager::cleanup()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ven::ImGuiWindowManager::render(Renderer* renderer, std::unordered_map<unsigned int, Object>& objects, std::unordered_map<unsigned int, Light>& lights, const ImGuiIO& io, Object& cameraObj, Camera& camera, KeyboardController& cameraController, VkPhysicalDevice physicalDevice, GlobalUbo& ubo)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderFrameWindow(io);

    ImGui::Begin("Debug Window");
    rendererSection(renderer, ubo);
    cameraSection(cameraObj, camera, cameraController);
    objectsSection(objects);
    lightsSection(lights);
    inputsSection(io);
    devicePropertiesSection(deviceProperties);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer->getCurrentCommandBuffer());
}

void ven::ImGuiWindowManager::renderFrameWindow(const ImGuiIO& io)
{
    const float framerate = io.Framerate;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_Always, ImVec2(0.0F, 0.0F));
    ImGui::Begin("Application Info", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::Text("FPS: %.1f", framerate);
    ImGui::Text("Frame time: %.3fms", 1000.0F / framerate);
    ImGui::End();
}

void ven::ImGuiWindowManager::rendererSection(Renderer *renderer, GlobalUbo& ubo)
{
    if (ImGui::CollapsingHeader("Renderer")) {
        ImGui::Text("Aspect Ratio: %.2f", renderer->getAspectRatio());

        if (ImGui::BeginTable("ClearColorTable", 2)) {
            ImGui::TableNextColumn();
            std::array<float, 4> clearColor = renderer->getClearColor();

            if (ImGui::ColorEdit4("Clear Color", clearColor.data())) {
                VkClearColorValue clearColorValue = {{clearColor[0], clearColor[1], clearColor[2], clearColor[3]}};
                renderer->setClearValue(clearColorValue);
            }

            ImGui::TableNextColumn();
            static int item_current = 0;

            if (ImGui::Combo("Color Presets##clearColor",
                             &item_current,
                             [](void*, int idx, const char** out_text) -> bool {
                                 if (idx < 0 || idx >= static_cast<int>(std::size(Colors::CLEAR_COLORS))) { return false; }
                                 *out_text = Colors::CLEAR_COLORS.at(static_cast<unsigned long>(idx)).first;
                                 return true;
                             },
                             nullptr,
                             std::size(Colors::CLEAR_COLORS))) {
                renderer->setClearValue(Colors::CLEAR_COLORS.at(static_cast<unsigned long>(item_current)).second);
            }

            ImGui::TableNextColumn();
            ImGui::ColorEdit4("Ambient Light Color", glm::value_ptr(ubo.ambientLightColor));
            ImGui::TableNextColumn();
            if (ImGui::Combo("Color Presets##ambientColor",
                             &item_current,
                             [](void*, int idx, const char** out_text) -> bool {
                                 if (idx < 0 || idx >= static_cast<int>(std::size(Colors::COLORS))) { return false; }
                                 *out_text = Colors::COLORS.at(static_cast<unsigned long>(idx)).first;
                                 return true;
                             },
                             nullptr,
                             std::size(Colors::COLORS))) {
                ubo.ambientLightColor = glm::vec4(Colors::COLORS.at(static_cast<unsigned long>(item_current)).second.r, Colors::COLORS.at(static_cast<unsigned long>(item_current)).second.g, Colors::COLORS.at(static_cast<unsigned long>(item_current)).second.b, 1.0F);

            }

            ImGui::TableNextColumn();
            ImGui::SliderFloat(("Intensity##" + std::to_string(0)).c_str(), &ubo.ambientLightColor.a, 0.0F, 1.0F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##ambientIntensity")) { ubo.ambientLightColor.a = DEFAULT_AMBIENT_LIGHT_INTENSITY; }

            ImGui::EndTable();
        }

        static bool fullscreen = false;
        if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
            renderer->getWindow().setFullscreen(fullscreen, renderer->getWindow().getExtent().width, renderer->getWindow().getExtent().height);
        }
    }
}

void ven::ImGuiWindowManager::cameraSection(Object &cameraObj, Camera &camera, KeyboardController &cameraController)
{
    if (ImGui::CollapsingHeader("Camera")) {
        float fov = camera.getFov();
        float near = camera.getNear();
        float far = camera.getFar();
        if (ImGui::BeginTable("CameraTable", 2)) {
            ImGui::TableNextColumn();
            ImGui::DragFloat3("Position", glm::value_ptr(cameraObj.transform3D.translation), 0.1F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##position")) { cameraObj.transform3D.translation = DEFAULT_POSITION; }

            ImGui::TableNextColumn();
            ImGui::DragFloat3("Rotation", glm::value_ptr(cameraObj.transform3D.rotation), 0.1F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##rotation")) { cameraObj.transform3D.rotation = DEFAULT_ROTATION; }

            ImGui::TableNextColumn();
            if (ImGui::SliderFloat("FOV", &fov, glm::radians(0.1F), glm::radians(180.0F))) { camera.setFov(fov); }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##fov")) { camera.setFov(DEFAULT_FOV); }

            ImGui::TableNextColumn();
            if (ImGui::SliderFloat("Near", &near, 0.001F, 10.0F)) { camera.setNear(near); }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##near")) { camera.setNear(DEFAULT_NEAR); }

            ImGui::TableNextColumn();
            if (ImGui::SliderFloat("Far", &far, 1.F, 1000.0F)) { camera.setFar(far); }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##far")) { camera.setFar(DEFAULT_FAR); }

            ImGui::TableNextColumn();
            ImGui::SliderFloat("Move Speed", &cameraController.m_moveSpeed, 0.1F, 10.0F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##moveSpeed")) { cameraController.m_moveSpeed = DEFAULT_MOVE_SPEED; }

            ImGui::TableNextColumn();
            ImGui::SliderFloat("Look Speed", &cameraController.m_lookSpeed, 0.1F, 10.0F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##lookSpeed")) { cameraController.m_lookSpeed = DEFAULT_LOOK_SPEED; }

            ImGui::EndTable();
        }
    }
}

void ven::ImGuiWindowManager::objectsSection(std::unordered_map<unsigned int, Object>& objects)
{
    if (ImGui::CollapsingHeader("Objects")) {
        ImVec4 color;
        bool open = false;

        for (auto& [id, object] : objects) {
            if (object.color.r == 0.0F && object.color.g == 0.0F && object.color.b == 0.0F) {
                color = { Colors::GRAY.r, Colors::GRAY.g, Colors::GRAY.b, 1.0F };
            } else {
                color = { object.color.r, object.color.g, object.color.b, 1.0F };
            }
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            open = ImGui::TreeNode(std::string(object.getName() + " [" + std::to_string(object.getId()) + "]").c_str());
            ImGui::PopStyleColor(1);
            if (open) {
                ImGui::DragFloat3(("Position##" + object.getName()).c_str(), glm::value_ptr(object.transform3D.translation), 0.1F);
                ImGui::DragFloat3(("Rotation##" + object.getName()).c_str(), glm::value_ptr(object.transform3D.rotation), 0.1F);
                ImGui::DragFloat3(("Scale##" + object.getName()).c_str(), glm::value_ptr(object.transform3D.scale), 0.1F);
                ImGui::Text("Address: %p", &object);
                ImGui::TreePop();
            }
        }
    }
}

void ven::ImGuiWindowManager::lightsSection(std::unordered_map<unsigned int, Light> &lights)
{
    if (ImGui::CollapsingHeader("Lights")) {
        bool open = false;

        for (auto& [id, light] : lights) {
            ImVec4 color{light.color.r, light.color.g, light.color.b, 1.0F};
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            open = ImGui::TreeNode(std::string(light.getName() + " [" + std::to_string(light.getId()) + "]").c_str());
            ImGui::PopStyleColor(1);
            if (open) {
                ImGui::Text("Address: %p", &light);
                ImGui::DragFloat3(("Position##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform3D.translation), 0.1F);
                ImGui::DragFloat3(("Rotation##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform3D.rotation), 0.1F);
                ImGui::DragFloat3(("Scale##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform3D.scale), 0.1F);
                if (ImGui::BeginTable("ColorTable", 2)) {
                    ImGui::TableNextColumn(); ImGui::ColorEdit4(("Color##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.color));

                    ImGui::TableNextColumn();
                    static int item_current = 0;
                    if (ImGui::Combo("Color Presets",
                                     &item_current,
                                     [](void*, const int idx, const char** out_text) -> bool {
                                         if (idx < 0 || idx >= static_cast<int>(std::size(Colors::COLORS))) { return false; }
                                         *out_text = Colors::COLORS.at(static_cast<unsigned long>(idx)).first;
                                         return true;
                                     },
                                     nullptr,
                                     std::size(Colors::COLORS))) {
                        light.color = {Colors::COLORS.at(static_cast<unsigned long>(item_current)).second, light.color.a};
                    }

                    ImGui::TableNextColumn();
                    ImGui::SliderFloat(("Intensity##" + std::to_string(light.getId())).c_str(), &light.color.a, 0.0F, 5.F);
                    ImGui::TableNextColumn();
                    if (ImGui::Button(("Reset##" + std::to_string(light.getId())).c_str())) { light.color.a = DEFAULT_LIGHT_INTENSITY; }

                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }
        }
    }
}

void ven::ImGuiWindowManager::inputsSection(const ImGuiIO &io)
{
    if (ImGui::CollapsingHeader("Input")) {
        ImGui::IsMousePosValid() ? ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y) : ImGui::Text("Mouse pos: <INVALID>");
        ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
        ImGui::Text("Mouse down:");
        for (int i = 0; i < static_cast<int>(std::size(io.MouseDown)); i++) {
            if (ImGui::IsMouseDown(i)) {
                ImGui::SameLine();
                ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
            }
        }
        ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        ImGui::Text("Keys down:");
        for (auto key = static_cast<ImGuiKey>(0); key < ImGuiKey_NamedKey_END; key = static_cast<ImGuiKey>(key + 1)) {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) { continue; }
            ImGui::SameLine();
            ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        }
    }
}

void ven::ImGuiWindowManager::devicePropertiesSection(VkPhysicalDeviceProperties deviceProperties)
{
    if (ImGui::CollapsingHeader("Device Properties")) {
        if (ImGui::BeginTable("DevicePropertiesTable", 2)) {

            ImGui::TableNextColumn(); ImGui::Text("Device Name: %s", deviceProperties.deviceName);
            ImGui::TableNextColumn(); ImGui::Text("API Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.apiVersion), VK_VERSION_MINOR(deviceProperties.apiVersion), VK_VERSION_PATCH(deviceProperties.apiVersion));
            ImGui::TableNextColumn(); ImGui::Text("Driver Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.driverVersion), VK_VERSION_MINOR(deviceProperties.driverVersion), VK_VERSION_PATCH(deviceProperties.driverVersion));
            ImGui::TableNextColumn();  ImGui::Text("Vendor ID: %d", deviceProperties.vendorID);
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
    }
}
