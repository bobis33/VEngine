#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui.h>
#include <imgui_internal.h>

#include "VEngine/Engine.hpp"


void ven::Engine::initImGui()
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
    if (vkCreateDescriptorPool(m_device.device(), &pool_info, nullptr, &imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool");
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(m_window.getGLFWindow(), true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_instance;
    init_info.PhysicalDevice = m_device.getPhysicalDevice();
    init_info.Device = m_device.device();
    init_info.Queue = m_device.graphicsQueue();
    init_info.DescriptorPool = imguiPool;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, m_renderer.getSwapChainRenderPass());
}

void ven::Engine::imGuiRender(ImGuiIO& io, Object& camera)
{
    float framerate = io.Framerate;
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(m_device.getPhysicalDevice(), &deviceProperties);

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
        std::array<float, 4> clearColor = m_renderer.getClearColor();
        if (ImGui::ColorEdit4("Clear Color", clearColor.data())) {
            m_renderer.setClearColor(clearColor);
        }
        ImGui::Text("Aspect Ratio: %.2f", m_renderer.getAspectRatio());
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
    ImGui::End();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_renderer.getCurrentCommandBuffer());
}

void ven::Engine::imGuiRenderDemo()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_renderer.getCurrentCommandBuffer());
}
