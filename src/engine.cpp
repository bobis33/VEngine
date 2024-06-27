#include <iostream>
#include <memory>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "VEngine/Engine.hpp"
#include "VEngine/RenderSystem.hpp"
#include "VEngine/Camera.hpp"
#include "VEngine/KeyboardController.hpp"

void ven::Engine::loadObjects()
{
    std::shared_ptr<Model> model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");

    Object cube = Object::createObject();
    cube.model = model;
    cube.transform3D.translation = {.0F, .0F, 2.5F};
    cube.transform3D.scale = {.5F, .5F, .5F};
    m_objects.push_back(std::move(cube));
}

ven::Engine::Engine(const uint32_t width, const uint32_t height, const std::string &title) : m_window(width, height, title)
{
    createInstance();
    createSurface();
    loadObjects();
}

void ven::Engine::mainLoop()
{
    RenderSystem renderSystem(m_device, m_renderer.getSwapChainRenderPass());
    Camera camera{};
    camera.setViewTarget(glm::vec3(-1.F, -2.F, -2.F), glm::vec3(0.F, 0.F, 2.5F));

    Object viewerObject = Object::createObject();
    KeyboardController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (glfwWindowShouldClose(m_window.getGLFWindow()) == 0)
    {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = glm::min(frameTime, 0.1F);

        cameraController.moveInPlaneXZ(m_window.getGLFWindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform3D.translation, viewerObject.transform3D.rotation);

        float aspect = m_renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0F), aspect, 0.1F, 10.F);

        if (VkCommandBuffer_T *commandBuffer = m_renderer.beginFrame())
        {
            m_renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(commandBuffer, m_objects, camera);
            Renderer::endSwapChainRenderPass(commandBuffer);
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
