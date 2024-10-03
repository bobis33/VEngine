#include "VEngine/ImGuiWindowManager.hpp"

void ven::ImGuiWindowManager::initImGui(GLFWwindow* window, VkInstance instance, Device* device, VkRenderPass renderPass)
{
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

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    VkDescriptorPool imguiPool;
    if (vkCreateDescriptorPool(device->device(), &pool_info, nullptr, &imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool");
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = device->getPhysicalDevice();
    init_info.Device = device->device();
    init_info.Queue = device->graphicsQueue();
    init_info.DescriptorPool = imguiPool;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, renderPass);
}

void ven::ImGuiWindowManager::imGuiRender(Renderer* renderer, std::unordered_map<id_t, Object>& objects, ImGuiIO& io, Object& camera, VkPhysicalDevice physicalDevice)
{
    float framerate = io.Framerate;
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

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
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camera.transform3D.translation.x, camera.transform3D.translation.y, camera.transform3D.translation.z);
        ImGui::Text("Camera Rotation: (%.2f, %.2f, %.2f)", camera.transform3D.rotation.x, camera.transform3D.rotation.y, camera.transform3D.rotation.z);
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
            ImGui::Text("Position: %.2f, %.2f, %.2f", object.transform3D.translation.x, object.transform3D.translation.y, object.transform3D.translation.z);
            ImGui::Text("Rotation: %.2f, %.2f, %.2f", object.transform3D.rotation.x, object.transform3D.rotation.y, object.transform3D.rotation.z);
            ImGui::Text("Scale: %.2f, %.2f, %.2f", object.transform3D.scale.x, object.transform3D.scale.y, object.transform3D.scale.z);
            ImGui::Text("Color: %.2f, %.2f, %.2f", object.color.x, object.color.y, object.color.z);
            ImGui::SameLine();
            ImGui::ColorButton("Color Button", ImVec4(object.color.x, object.color.y, object.color.z, 1.0f), ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
            if (object.pointLight != nullptr) {
                ImGui::Text("Intensity: %.2f", object.pointLight->lightIntensity);
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