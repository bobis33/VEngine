#include <iostream>
#include <memory>
#include <array>

#include "VEngine/Engine.hpp"

ven::Engine::Engine(int width, int height, const std::string &title)  : m_window(width, height, title)
{
    initVulkan();
    createPipelineLayout();
    createPiepeline();
    createCommandBuffers();
}

ven::Engine::~Engine()
{
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void ven::Engine::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }

}

void ven::Engine::createPiepeline()
{
    PipelineConfigInfo pipelineConfig = Shaders::defaultPipelineConfigInfo(m_swapChain.width(), m_swapChain.height());
    pipelineConfig.renderPass = m_swapChain.getRenderPass();
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_shaders = std::make_unique<Shaders>(m_device, "shaders/bin/vertex.spv", "shaders/bin/fragment.spv", pipelineConfig);
}

void ven::Engine::createCommandBuffers()
{
    m_commandBuffers.resize(m_swapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }

    for (int i = 0; i < m_commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain.getRenderPass();
        renderPassInfo.framebuffer = m_swapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.1F, 0.1F, 0.1F, 1.0F}};
        clearValues[1].depthStencil = {1.0F, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_shaders->bind(m_commandBuffers[i]);
        vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(m_commandBuffers[i]);

        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}

void ven::Engine::drawFrame()
{
    uint32_t imageIndex = 0;
    VkResult result = m_swapChain.acquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    if (m_swapChain.submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit command buffer");
    }
}

void ven::Engine::deleteResources()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void ven::Engine::mainLoop()
{
    while (glfwWindowShouldClose(m_window.getGLFWindow()) == 0)
    {
        glfwPollEvents();
        drawFrame();
        //glfwSwapBuffers(m_window.getGLFWindow());
    }
    vkDeviceWaitIdle(m_device.device());
    deleteResources();
}

void ven::Engine::initVulkan()
{
    createInstance();
    createSurface();
    if (m_instance == nullptr || m_surface == nullptr)
    {
        throw std::runtime_error("Failed to create Vulkan instance or surface");
    }
}

void ven::Engine::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VEngine App";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.pEngineName = "VEngine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void ven::Engine::createSurface()
{
    if (glfwCreateWindowSurface(m_instance, m_window.getGLFWindow(), nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
}
