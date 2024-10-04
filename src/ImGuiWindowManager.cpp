#include <glm/gtc/type_ptr.hpp>

#include "VEngine/ImGuiWindowManager.hpp"

void ven::ImGuiWindowManager::initImGui(GLFWwindow* window, VkInstance instance, Device* device, VkRenderPass renderPass)
{
    VkDescriptorPool pool;

    ImGui::CreateContext();
    // ImGui::StyleColorsDark();

    VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            nullptr,
            VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            1000,
            std::size(pool_sizes),
            pool_sizes
    };

    if (vkCreateDescriptorPool(device->device(), &pool_info, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool");
    }
    ImGui_ImplVulkan_InitInfo init_info = {
            .Instance = instance,
            .PhysicalDevice = device->getPhysicalDevice(),
            .Device = device->device(),
            .Queue = device->graphicsQueue(),
            .DescriptorPool = pool,
            .MinImageCount = 3,
            .ImageCount = 3,
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT
    };

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_Init(&init_info, renderPass);
}

void ven::ImGuiWindowManager::imGuiRender(Renderer* renderer, std::unordered_map<id_t, Object>& objects, ImGuiIO& io, Object& cameraObj, Camera& camera, KeyboardController& cameraController, VkPhysicalDevice physicalDevice)
{
    float framerate = io.Framerate;
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    float fov = camera.getFov();

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Application Info");
    ImGui::Text("FPS: %.1f", framerate);
    ImGui::Text("Frame time: %.3fms", 1000.0f / framerate);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1300, 0));
    ImGui::Begin("ImGui Debug Window");

    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraObj.transform3D.translation), 0.1F);
        ImGui::DragFloat3("Camera Rotation", glm::value_ptr(cameraObj.transform3D.rotation), 0.1F);
        if (ImGui::SliderFloat("Camera FOV", &fov, 0.1F, glm::radians(180.0F))) {
            camera.setFov(fov);
        }
        if (ImGui::SliderFloat("Move Speed", &cameraController.m_moveSpeed, 0.1F, 10.0F)) {
            cameraController.m_moveSpeed = glm::clamp(cameraController.m_moveSpeed, 0.1F, 10.0F);
        }
        if (ImGui::SliderFloat("Look Speed", &cameraController.m_lookSpeed, 0.1F, 10.0F)) {
            cameraController.m_lookSpeed = glm::clamp(cameraController.m_lookSpeed, 0.1F, 10.0F);
        }
        if (ImGui::Button("Reset position")) {
            cameraObj.transform3D.translation = glm::vec3(0.0F, 0.0F, -2.5F);
            cameraObj.transform3D.rotation = glm::vec3(0.0F, 0.0F, 0.0F);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset FOV")) {
            camera.setFov(DEFAULT_FOV);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset move speed")) {
            cameraController.m_moveSpeed = DEFAULT_MOVE_SPEED;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset look speed")) {
            cameraController.m_lookSpeed = DEFAULT_LOOK_SPEED;
        }

    }

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
        struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exist
        ImGui::Text("Keys down:");
        for (ImGuiKey key = static_cast<ImGuiKey>(0); key < ImGuiKey_NamedKey_END; key = static_cast<ImGuiKey>(key + 1)) {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) { continue; }
            ImGui::SameLine();
            ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        }
    }

    if (ImGui::CollapsingHeader("Render Settings")) {
        std::array<float, 4> clearColor = renderer->getClearColor();
        if (ImGui::ColorEdit4("Clear Color", clearColor.data())) {
            renderer->setClearColor(clearColor);
        }
        ImGui::Text("Aspect Ratio: %.2f", renderer->getAspectRatio());
    }

    if (ImGui::CollapsingHeader("Device Properties")) {
        ImGui::Text("Device Name: %s", deviceProperties.deviceName);
        ImGui::Text("API Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.apiVersion), VK_VERSION_MINOR(deviceProperties.apiVersion), VK_VERSION_PATCH(deviceProperties.apiVersion));
        ImGui::Text("Driver Version: %d.%d.%d", VK_VERSION_MAJOR(deviceProperties.driverVersion), VK_VERSION_MINOR(deviceProperties.driverVersion), VK_VERSION_PATCH(deviceProperties.driverVersion));
        ImGui::Text("Vendor ID: %d", deviceProperties.vendorID);
        ImGui::Text("Device ID: %d", deviceProperties.deviceID);
        ImGui::Text("Device Type: %d", deviceProperties.deviceType);
        ImGui::Text("Discrete Queue Priorities: %d", deviceProperties.limits.discreteQueuePriorities);
        ImGui::Text("Max Push Constants Size: %d", deviceProperties.limits.maxPushConstantsSize);
        ImGui::Text("Max Memory Allocation Count: %d", deviceProperties.limits.maxMemoryAllocationCount);
        ImGui::Text("Max Image Dimension 1D: %d", deviceProperties.limits.maxImageDimension1D);
        ImGui::Text("Max Image Dimension 2D: %d", deviceProperties.limits.maxImageDimension2D);
        ImGui::Text("Max Image Dimension 3D: %d", deviceProperties.limits.maxImageDimension3D);
        ImGui::Text("Max Image Dimension Cube: %d", deviceProperties.limits.maxImageDimensionCube);
        ImGui::Text("Max Image Array Layers: %d", deviceProperties.limits.maxImageArrayLayers);
        ImGui::Text("Max Texel Buffer Elements: %d", deviceProperties.limits.maxTexelBufferElements);
        ImGui::Text("Max Uniform Buffer Range: %d", deviceProperties.limits.maxUniformBufferRange);
        ImGui::Text("Max Storage Buffer Range: %d", deviceProperties.limits.maxStorageBufferRange);
    }

    ImGui::SetNextWindowPos(ImVec2(0, 65));
    ImGui::Begin("Objects");
    for (auto& [id, object] : objects) {
        if (ImGui::CollapsingHeader(object.name.c_str())) {
            ImGui::DragFloat3(("Position##" + object.name).c_str(), glm::value_ptr(object.transform3D.translation), 0.1F);
            ImGui::DragFloat3(("Rotation##" + object.name).c_str(), glm::value_ptr(object.transform3D.rotation), 0.1F);
            ImGui::DragFloat3(("Scale##" + object.name).c_str(), glm::value_ptr(object.transform3D.scale), 0.1F);
            ImGui::ColorEdit3(("Color##" + object.name).c_str(), glm::value_ptr(object.color));
            if (object.pointLight != nullptr) {
                ImGui::SliderFloat(("Intensity##" + object.name).c_str(), &object.pointLight->lightIntensity, 0.0F, 10.0F);
            }
            ImGui::Text("ID: %d", object.getId());
            ImGui::Text("Address: %p", &object);
        }
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer->getCurrentCommandBuffer());
}

void ven::ImGuiWindowManager::imGuiRenderDemo(Renderer* renderer)
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer->getCurrentCommandBuffer());
}