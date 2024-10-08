#include <chrono>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "VEngine/Engine.hpp"
#include "VEngine/KeyboardController.hpp"
#include "VEngine/System/RenderSystem.hpp"
#include "VEngine/System/PointLightSystem.hpp"
#include "VEngine/Descriptors/DescriptorWriter.hpp"
#include "VEngine/ImGuiWindowManager.hpp"
#include "VEngine/Colors.hpp"

ven::Engine::Engine(const uint32_t width, const uint32_t height, const std::string &title) : m_window(width, height, title)
{
    createInstance();
    createSurface();
    ImGuiWindowManager::init(m_window.getGLFWindow(), m_instance, &m_device, m_renderer.getSwapChainRenderPass());
    m_globalPool = DescriptorPool::Builder(m_device).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT).build();
    loadObjects();
}

void ven::Engine::createInstance()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = nullptr;
    VkInstanceCreateInfo createInfo{};
    VkApplicationInfo appInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pNext = nullptr, .pApplicationName = "VEngine App", .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0), .pEngineName = "VEngine", .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0), .apiVersion = VK_API_VERSION_1_0 };

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void ven::Engine::loadObjects()
{
    std::shared_ptr model = Model::createModelFromFile(m_device, "models/flat_vase.obj");

    Object flatVase = Object::createObject();
    flatVase.name = "flat vase";
    flatVase.model = model;
    flatVase.transform3D.translation = {-.5F, .5F, 0.F};
    flatVase.transform3D.scale = {3.F, 1.5F, 3.F};
    m_objects.emplace(flatVase.getId(), std::move(flatVase));

    model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");
    Object smoothVase = Object::createObject();
    smoothVase.name = "smooth vase";
    smoothVase.model = model;
    smoothVase.transform3D.translation = {.5F, .5F, 0.F};
    smoothVase.transform3D.scale = {3.F, 1.5F, 3.F};
    m_objects.emplace(smoothVase.getId(), std::move(smoothVase));

    model = Model::createModelFromFile(m_device, "models/quad.obj");
    Object floor = Object::createObject();
    floor.name = "floor";
    floor.model = model;
    floor.transform3D.translation = {0.F, .5F, 0.F};
    floor.transform3D.scale = {3.F, 1.F, 3.F};
    m_objects.emplace(floor.getId(), std::move(floor));

    const std::vector<glm::vec3> lightColors{
            {Colors::RED},
            {Colors::GREEN},
            {Colors::BLUE},
            {Colors::YELLOW},
            {Colors::CYAN},
            {Colors::MAGENTA}
    };

    for (std::size_t i = 0; i < lightColors.size(); i++)
    {
        Object pointLight = Object::makePointLight();
        pointLight.name = "point light " + std::to_string(i);
        pointLight.color = lightColors[i];
        auto rotateLight = rotate(glm::mat4(1.F), (static_cast<float>(i) * glm::two_pi<float>()) / static_cast<float>(lightColors.size()), {0.F, -1.F, 0.F});
        pointLight.transform3D.translation = glm::vec3(rotateLight * glm::vec4(-1.F, -1.F, -1.F, 1.F));
        m_objects.emplace(pointLight.getId(), std::move(pointLight));
    }
}

void ven::Engine::mainLoop()
{
    GlobalUbo ubo{};
    Camera camera{};
    KeyboardController cameraController{};
    std::chrono::duration<float> deltaTime{};
    VkCommandBuffer_T *commandBuffer = nullptr;
    bool showDebugWindow = true;
    float frameTime = NAN;
    int frameIndex = 0;
    Object viewerObject = Object::createObject();
    std::chrono::time_point<std::chrono::system_clock> newTime;
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::high_resolution_clock::now();
    std::unique_ptr<DescriptorSetLayout> globalSetLayout = DescriptorSetLayout::Builder(m_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    RenderSystem renderSystem(m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
    PointLightSystem pointLightSystem(m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
    ImGuiIO &io = ImGui::GetIO();
    VkDescriptorBufferInfo bufferInfo{};

    for (auto& uboBuffer : uboBuffers)
    {
        uboBuffer = std::make_unique<Buffer>(m_device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffer->map();
    }
    for (std::size_t i = 0; i < globalDescriptorSets.size(); i++) {
        bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *m_globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }
    camera.setViewTarget(glm::vec3(-1.F, -2.F, -2.F), glm::vec3(0.F, 0.F, 2.5F));
    viewerObject.transform3D.translation.z = DEFAULT_POSITION[2];

    m_renderer.setClearValue();

    while (glfwWindowShouldClose(m_window.getGLFWindow()) == 0)
    {
        glfwPollEvents();

        newTime = std::chrono::high_resolution_clock::now();
        deltaTime = newTime - currentTime;
        currentTime = newTime;
        frameTime = deltaTime.count();
        commandBuffer = m_renderer.beginFrame();

        cameraController.moveInPlaneXZ(m_window.getGLFWindow(), frameTime, viewerObject, &showDebugWindow);
        camera.setViewYXZ(viewerObject.transform3D.translation, viewerObject.transform3D.rotation);
        camera.setPerspectiveProjection(m_renderer.getAspectRatio());

        if (commandBuffer != nullptr) {
            frameIndex = m_renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[static_cast<unsigned long>(frameIndex)], m_objects};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            PointLightSystem::update(frameInfo, ubo);
            uboBuffers[static_cast<unsigned long>(frameIndex)]->writeToBuffer(&ubo);
            uboBuffers[static_cast<unsigned long>(frameIndex)]->flush();

            m_renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);
            renderSystem.renderObjects(frameInfo);
            pointLightSystem.render(frameInfo);

            if (showDebugWindow) { ImGuiWindowManager::render(&m_renderer, m_objects, io, viewerObject, camera, cameraController, m_device.getPhysicalDevice(), ubo); }

            m_renderer.endSwapChainRenderPass(commandBuffer);
            m_renderer.endFrame();
            commandBuffer = nullptr;
        }
    }
    ImGuiWindowManager::cleanup();
    vkDeviceWaitIdle(m_device.device());
}
