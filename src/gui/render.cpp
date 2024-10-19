#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <glm/gtc/type_ptr.hpp>

#include "VEngine/Gui.hpp"
#include "VEngine/Utils/Colors.hpp"

void ven::Gui::cleanup()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ven::Gui::render(Renderer* renderer,  SceneManager& sceneManager, Camera& camera, const VkPhysicalDevice physicalDevice, GlobalUbo& ubo, const ClockData& clockData)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    renderFrameWindow(clockData);

    ImGui::Begin("Debug Window");
    rendererSection(renderer, ubo);
    cameraSection(camera);
    lightsSection(sceneManager);
    objectsSection(sceneManager);
    inputsSection(*m_io);
    devicePropertiesSection(deviceProperties);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer->getCurrentCommandBuffer());
}

void ven::Gui::renderFrameWindow(const ClockData& clockData)
{
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_Always, ImVec2(0.0F, 0.0F));
    ImGui::Begin("Application Info", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::Text("FPS: %.1f", clockData.fps);
    ImGui::Text("Frame time: %.3fms", clockData.deltaTimeMS);
    ImGui::End();
}

void ven::Gui::rendererSection(Renderer *renderer, GlobalUbo& ubo)
{
    if (ImGui::CollapsingHeader("Renderer")) {
        ImGui::Text("Aspect Ratio: %.2f", renderer->getAspectRatio());

        if (ImGui::BeginTable("ClearColorTable", 2)) {
            ImGui::TableNextColumn();
            std::array<float, 4> clearColor = renderer->getClearColor();

            if (ImGui::ColorEdit4("Clear Color", clearColor.data())) {
                const VkClearColorValue clearColorValue = {{clearColor[0], clearColor[1], clearColor[2], clearColor[3]}};
                renderer->setClearValue(clearColorValue);
            }

            ImGui::TableNextColumn();
            static int item_current = 0;

            if (ImGui::Combo("Color Presets##clearColor",
                             &item_current,
                             [](void*, const int idx, const char** out_text) -> bool {
                                 if (idx < 0 || idx >= static_cast<int>(std::size(Colors::COLOR_PRESETS_VK))) { return false; }
                                 *out_text = Colors::COLOR_PRESETS_VK.at(static_cast<unsigned long>(idx)).first;
                                 return true;
                             },
                             nullptr,
                             std::size(Colors::COLOR_PRESETS_VK))) {
                renderer->setClearValue(Colors::COLOR_PRESETS_VK.at(static_cast<unsigned long>(item_current)).second);
                             }

            ImGui::TableNextColumn();
            ImGui::ColorEdit4("Ambient Light Color", glm::value_ptr(ubo.ambientLightColor));
            ImGui::TableNextColumn();
            if (ImGui::Combo("Color Presets##ambientColor",
                             &item_current,
                             [](void*, const int idx, const char** out_text) -> bool {
                                 if (idx < 0 || idx >= static_cast<int>(std::size(Colors::COLOR_PRESETS_4))) { return false; }
                                 *out_text = Colors::COLOR_PRESETS_4.at(static_cast<unsigned long>(idx)).first;
                                 return true;
                             },
                             nullptr,
                             std::size(Colors::COLOR_PRESETS_4))) {
                ubo.ambientLightColor = Colors::COLOR_PRESETS_4.at(static_cast<unsigned long>(item_current)).second;
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

void ven::Gui::cameraSection(Camera &camera)
{
    if (ImGui::CollapsingHeader("Camera")) {
        float fov = camera.getFov();
        float near = camera.getNear();
        float far = camera.getFar();
        if (ImGui::BeginTable("CameraTable", 2)) {
            ImGui::TableNextColumn();
            ImGui::DragFloat3("Position", glm::value_ptr(camera.transform.translation), 0.1F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##position")) { camera.transform.translation = DEFAULT_POSITION; }

            ImGui::TableNextColumn();
            ImGui::DragFloat3("Rotation", glm::value_ptr(camera.transform.rotation), 0.1F);
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##rotation")) { camera.transform.rotation = DEFAULT_ROTATION; }

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
            float moveSpeed = camera.getMoveSpeed();
            if (ImGui::SliderFloat("Move speed", &moveSpeed, 0.1F, 10.0F)) { camera.setMoveSpeed(moveSpeed); }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##moveSpeed")) { camera.setMoveSpeed(DEFAULT_MOVE_SPEED); }

            ImGui::TableNextColumn();
            float lookSpeed = camera.getLookSpeed();
            if (ImGui::SliderFloat("Look speed", &lookSpeed, 0.1F, 10.0F)) { camera.setLookSpeed(lookSpeed); }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset##lookSpeed")) { camera.setLookSpeed(DEFAULT_LOOK_SPEED); }

            ImGui::EndTable();
        }
    }
}

void ven::Gui::objectsSection(SceneManager& sceneManager)
{
    if (ImGui::CollapsingHeader("Objects")) {
        bool open = false;
        for (Object::Map& objects = sceneManager.getObjects(); auto& [id, object] : objects) {
            ImGui::PushStyleColor(ImGuiCol_Text, { Colors::GRAY_4.r, Colors::GRAY_4.g, Colors::GRAY_4.b, 1.0F });
            open = ImGui::TreeNode(std::string(object.getName() + " [" + std::to_string(object.getId()) + "]").c_str());
            ImGui::PopStyleColor(1);
            if (open) {
                if (ImGui::Button(("Delete##" + object.getName()).c_str())) {
                    m_objectsToRemove.push_back(id);
                    sceneManager.setDestroyState(true);
                }
                ImGui::SameLine();
                if (ImGui::Button(("Duplicate##" + object.getName()).c_str())) {
                    sceneManager.duplicateObject(id);
                }
                ImGui::Text("Address: %p", static_cast<void*>(&object));
                ImGui::DragFloat3(("Position##" + object.getName()).c_str(), glm::value_ptr(object.transform.translation), 0.1F);
                ImGui::DragFloat3(("Rotation##" + object.getName()).c_str(), glm::value_ptr(object.transform.rotation), 0.1F);
                ImGui::DragFloat3(("Scale##" + object.getName()).c_str(), glm::value_ptr(object.transform.scale), 0.1F);
                ImGui::TreePop();
            }
        }
    }
}

void ven::Gui::lightsSection(SceneManager& sceneManager)
{

    if (ImGui::CollapsingHeader("Lights")) {
        bool open = false;
        float tempIntensity = m_intensity;
        float tempShininess = m_shininess;
        Light::Map& lights = sceneManager.getLights();

        if (ImGui::BeginTable("LightTable", 2)) {
            ImGui::TableNextColumn();
            if (ImGui::SliderFloat("Global Intensity", &tempIntensity, 0.0F, 5.F)) {
                m_intensity = tempIntensity;
                for (auto&[fst, snd] : lights) {
                    snd.color.a = m_intensity;
                }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("Reset")) {
                m_intensity = DEFAULT_LIGHT_INTENSITY;
                tempIntensity = m_intensity;
                for (auto&[fst, snd] : lights) {
                    snd.color.a = m_intensity;
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::SliderFloat("Global Shininess", &tempShininess, 0.0F, 512.F)) {
                m_shininess = tempShininess;
                for (auto&[fst, snd] : lights) {
                    snd.setShininess(m_shininess);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("Reset")) {
                m_shininess = DEFAULT_SHININESS;
                tempShininess = m_shininess;
                for (auto&[fst, snd] : lights) {
                    snd.setShininess(m_shininess);
                }
            }

            ImGui::EndTable();
        }

        for (auto& [id, light] : lights) {
            ImGui::PushStyleColor(ImGuiCol_Text, {light.color.r, light.color.g, light.color.b, 1.0F});
            open = ImGui::TreeNode(std::string(light.getName() + " [" + std::to_string(light.getId()) + "]").c_str());
            ImGui::PopStyleColor(1);
            if (open) {
                if (ImGui::Button(("Delete##" + light.getName()).c_str())) {
                    m_lightsToRemove.push_back(id);
                    sceneManager.setDestroyState(true);
                }
                ImGui::SameLine();
                if (ImGui::Button(("Duplicate##" + light.getName()).c_str())) {
                    sceneManager.duplicateLight(id);
                }
                ImGui::Text("Address: %p", static_cast<void*>(&light));
                ImGui::DragFloat3(("Position##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform.translation), 0.1F);
                ImGui::DragFloat3(("Rotation##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform.rotation), 0.1F);
                ImGui::DragFloat3(("Scale##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.transform.scale), 0.1F);
                if (ImGui::BeginTable("ColorTable", 2)) {
                    ImGui::TableNextColumn();
                    ImGui::ColorEdit4(("Color##" + std::to_string(light.getId())).c_str(), glm::value_ptr(light.color));
                    ImGui::TableNextColumn();
                    static int item_current = 0;
                    if (ImGui::Combo("Color Presets",
                                     &item_current,
                                     [](void*, const int idx, const char** out_text) -> bool {
                                         if (idx < 0 || idx >= static_cast<int>(std::size(Colors::COLOR_PRESETS_3))) { return false; }
                                         *out_text = Colors::COLOR_PRESETS_3.at(static_cast<unsigned long>(idx)).first;
                                         return true;
                                     },
                                     nullptr,
                                     std::size(Colors::COLOR_PRESETS_3))) {
                        light.color = {Colors::COLOR_PRESETS_3.at(static_cast<unsigned long>(item_current)).second, light.color.a};
                    }
                    ImGui::EndTable();

                    ImGui::SliderFloat(("Intensity##" + std::to_string(light.getId())).c_str(), &light.color.a, 0.0F, 5.F);
                    ImGui::SameLine();
                    if (ImGui::Button(("Reset##" + std::to_string(light.getId())).c_str())) { light.color.a = DEFAULT_LIGHT_INTENSITY; }
                    float shininess = light.getShininess();
                    if (ImGui::SliderFloat("Shininess", &shininess, 0.0F, 512.F)) {
                        light.setShininess(shininess);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Reset##shininess")) { light.setShininess(DEFAULT_SHININESS); }
                }
                ImGui::TreePop();
            }
        }
    }
}

void ven::Gui::inputsSection(const ImGuiIO& io)
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

void ven::Gui::devicePropertiesSection(VkPhysicalDeviceProperties deviceProperties)
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
