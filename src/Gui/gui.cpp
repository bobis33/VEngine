#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include "VEngine/Gui/Gui.hpp"

void createDescriptorPool(const VkDevice& device, VkDescriptorPool& pool) {
    static constexpr uint16_t DESCRIPTOR_COUNT = 1000;
    static constexpr std::array<VkDescriptorPoolSize, 11> poolSizes = {{
        { .type=VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount=DESCRIPTOR_COUNT},
        { .type=VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, .descriptorCount=DESCRIPTOR_COUNT },
        { .type=VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, .descriptorCount=DESCRIPTOR_COUNT }}};
    static constexpr VkDescriptorPoolCreateInfo poolInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets = DESCRIPTOR_COUNT,
        .poolSizeCount = poolSizes.size(),
        .pPoolSizes = poolSizes.data()
    };
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw utl::THROW_ERROR("Failed to create ImGui descriptor pool");
    }
}

ven::Gui::~Gui() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(m_device, m_pool, nullptr);
}

ven::Gui::Gui(const Device& device, Camera& camera, GLFWwindow* window, const VkRenderPass& renderPass, std::vector<Model>& models, std::array<VkClearValue, 2>& clearValues, glm::vec3& ambientColor): m_device(device.getVkDevice()), m_models(models), m_clearValues(clearValues), m_ambientColor(ambientColor), m_camera(camera) {
    IMGUI_CHECKVERSION();
    const VkPhysicalDevice &physicalDevice = device.getPhysicalDevice();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    createDescriptorPool(m_device, m_pool);
    ImGui_ImplVulkan_InitInfo init_info{};
    init_info.Instance = device.getVkInstance();
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = m_device;
    init_info.QueueFamily = device.findQueueFamilies(physicalDevice).graphicsFamily.value();
    init_info.Queue = device.getGraphicsQueue();
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = m_pool;
    init_info.RenderPass = renderPass;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.MSAASamples = device.getMsaaSamples();
    init_info.Subpass = 0;
    ImGui_ImplVulkan_Init(&init_info);
    ImGui_ImplGlfw_InitForVulkan(window, true);
    vkGetPhysicalDeviceProperties(physicalDevice, &m_deviceProperties);
    //blackRedTheme();
}
