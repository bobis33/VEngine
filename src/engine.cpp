#include <iostream>
#include <memory>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "VEngine/Engine.hpp"
#include "VEngine/RenderSystem.hpp"

ven::Engine::Engine(int width, int height, const std::string &title) : m_window(width, height, title)
{
    createInstance();
    createSurface();
    loadObjects();
}

ven::Engine::~Engine() {}


void ven::Engine::mainLoop()
{
    RenderSystem renderSystem(m_device, m_renderer.getSwapChainRenderPass());
    while (glfwWindowShouldClose(m_window.getGLFWindow()) == 0)
    {
        glfwPollEvents();
        if (auto commandBuffer = m_renderer.beginFrame())
        {
            m_renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(commandBuffer, m_objects);
            m_renderer.endSwapChainRenderPass(commandBuffer);
            m_renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(m_device.device());
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

void ven::Engine::loadObjects()
{
    std::vector<Model::Vertex> vertices {
        {{0.0F, -0.5F}, {1.0F, 0.0F, 0.0F}},
        {{0.5F, 0.5F}, {0.0F, 1.0F, 0.0F}},
        {{-0.5F, 0.5F}, {0.0F, 0.0F, 1.0F}}
    };

    auto model = std::make_shared<Model>(m_device, vertices);

    auto triangle = ven::Object::createObject();
    triangle.model = model;
    triangle.color = {.1F, .8F, .1F};
    triangle.transform2d.translation.x = .2F;
    triangle.transform2d.scale = {2.F, .5F};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();
    m_objects.push_back(std::move(triangle));
}
