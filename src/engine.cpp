#include <iostream>
#include <memory>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "VEngine/Buffer.hpp"
#include "VEngine/Camera.hpp"
#include "VEngine/Engine.hpp"
#include "VEngine/KeyboardController.hpp"
#include "VEngine/RenderSystem.hpp"

struct GlobalUbo
{
    glm::mat4 projectionView{1.F};
    glm::vec4 ambientLightColor{1.F, 1.F, 1.F, .02F};
    glm::vec3 lightPosition{-1.F};
    alignas(16) glm::vec4 lightColor{1.F};
};

void ven::Engine::loadObjects()
{
    std::shared_ptr<Model> model = Model::createModelFromFile(m_device, "models/flat_vase.obj");

    Object flatVase = Object::createObject();
    flatVase.model = model;
    flatVase.transform3D.translation = {-.5F, .5F, 0.F};
    flatVase.transform3D.scale = {3.F, 1.5F, 3.F};
    m_objects.push_back(std::move(flatVase));

    model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");
    Object smoothVase = Object::createObject();
    smoothVase.model = model;
    smoothVase.transform3D.translation = {.5F, .5F, 0.F};
    smoothVase.transform3D.scale = {3.F, 1.5F, 3.F};
    m_objects.push_back(std::move(smoothVase));

    model = Model::createModelFromFile(m_device, "models/quad.obj");
    Object quad = Object::createObject();
    quad.model = model;
    quad.transform3D.translation = {0.F, .5F, 0.F};
    smoothVase.transform3D.scale = {3.F, 1.F, 3.F};
    m_objects.push_back(std::move(quad));
}

ven::Engine::Engine(const uint32_t width, const uint32_t height, const std::string &title) : m_window(width, height, title)
{
    createInstance();
    createSurface();
    m_globalPool = DescriptorPool::Builder(m_device).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT).build();
    loadObjects();
}

void ven::Engine::mainLoop()
{
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (auto & uboBuffer : uboBuffers)
    {
        uboBuffer = std::make_unique<Buffer>(m_device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffer->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(m_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT).build();

    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *m_globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    RenderSystem renderSystem(m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
    Camera camera{};
    camera.setViewTarget(glm::vec3(-1.F, -2.F, -2.F), glm::vec3(0.F, 0.F, 2.5F));

    Object viewerObject = Object::createObject();
    viewerObject.transform3D.translation.z = -2.5F;
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
        camera.setPerspectiveProjection(glm::radians(50.0F), aspect, 0.1F, 100.F);

        if (VkCommandBuffer_T *commandBuffer = m_renderer.beginFrame())
        {
            int frameIndex = (m_renderer.getFrameIndex());
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

            GlobalUbo ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            m_renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(frameInfo, m_objects);
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
