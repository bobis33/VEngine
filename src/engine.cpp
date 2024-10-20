#include "VEngine/Engine.hpp"
#include "VEngine/EventManager.hpp"
#include "VEngine/Descriptors/DescriptorWriter.hpp"
#include "VEngine/RenderSystem/ObjectRenderSystem.hpp"
#include "VEngine/RenderSystem/PointLightRenderSystem.hpp"
#include "VEngine/Utils/Colors.hpp"
#include "VEngine/Utils/Clock.hpp"

ven::Engine::Engine(const uint32_t width, const uint32_t height, const std::string &title) : m_state(EDITOR), m_window(width, height, title) {
    m_gui.init(m_window.getGLFWindow(), m_device.getInstance(), &m_device, m_renderer.getSwapChainRenderPass());
    m_globalPool = DescriptorPool::Builder(m_device).setMaxSets(MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT).build();

    m_framePools.resize(MAX_FRAMES_IN_FLIGHT);
    const auto framePoolBuilder = DescriptorPool::Builder(m_device)
                                .setMaxSets(1000)
                                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
                                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
                                .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
    for (auto & framePool : m_framePools) {
        framePool = framePoolBuilder.build();
    }

    loadObjects();
}

void ven::Engine::loadObjects()
{
    constexpr std::array lightColors{
        Colors::RED_4,
        Colors::GREEN_4,
        Colors::BLUE_4,
        Colors::YELLOW_4,
        Colors::CYAN_4,
        Colors::MAGENTA_4
    };
    auto& quad = m_sceneManager.createObject();
    quad.setName("quad");
    quad.setModel(Model::createModelFromFile(m_device, "assets/models/quad.obj"));
    quad.transform.translation = {0.F, .5F, 0.F};
    quad.transform.scale = {3.F, 1.F, 3.F};

    auto& flatVase = m_sceneManager.createObject();
    flatVase.setName("flat vase");
    flatVase.setModel(Model::createModelFromFile(m_device, "assets/models/flat_vase.obj"));
    flatVase.transform.translation = {-.5F, .5F, 0.F};
    flatVase.transform.scale = {3.F, 1.5F, 3.F};

    auto& smoothVase = m_sceneManager.createObject();
    smoothVase.setName("smooth vase");
    smoothVase.setModel(Model::createModelFromFile(m_device, "assets/models/smooth_vase.obj"));
    smoothVase.transform.translation = {.5F, .5F, 0.F};
    smoothVase.transform.scale = {3.F, 1.5F, 3.F};

    for (std::size_t i = 0; i < lightColors.size(); i++)
    {
        glm::mat4 rotateLight = rotate(
            glm::mat4(1.F),
            static_cast<float>(i) * glm::two_pi<float>() / static_cast<float>(lightColors.size()),
            {0.F, -1.F, 0.F}
        );
        auto& pointLight = m_sceneManager.createLight();
        pointLight.color = lightColors.at(i);
        pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.F, -1.F, -1.F, 1.F));
    }
}

void ven::Engine::mainLoop()
{
    Clock clock;
    Camera camera{};
    EventManager eventManager{};
    GlobalUbo ubo{};
    VkCommandBuffer_T *commandBuffer = nullptr;
    VkDescriptorBufferInfo bufferInfo{};
    float frameTime = 0.0F;
    unsigned long frameIndex = 0;
    std::unique_ptr globalSetLayout(DescriptorSetLayout::Builder(m_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build());
    std::vector<std::unique_ptr<Buffer>> uboBuffers(MAX_FRAMES_IN_FLIGHT);
    std::vector<VkDescriptorSet> globalDescriptorSets(MAX_FRAMES_IN_FLIGHT);
    ObjectRenderSystem objectRenderSystem(m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
    PointLightRenderSystem pointLightRenderSystem(m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

    for (auto& uboBuffer : uboBuffers)
    {
        uboBuffer = std::make_unique<Buffer>(m_device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffer->map();
    }
    for (std::size_t i = 0; i < globalDescriptorSets.size(); i++) {
        bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *m_globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    while (m_state != EXIT)
    {
        clock.update();
        frameTime = clock.getDeltaTime();
        eventManager.handleEvents(m_window.getGLFWindow(), &m_state, camera, m_gui, frameTime);
        commandBuffer = m_renderer.beginFrame();

        camera.setViewXYZ(camera.transform.translation, camera.transform.rotation);
        camera.setPerspectiveProjection(m_renderer.getAspectRatio());

        if (commandBuffer != nullptr) {
            frameIndex = m_renderer.getFrameIndex();
            m_framePools[frameIndex]->resetPool();
            FrameInfo frameInfo{
                .frameIndex=frameIndex,
                .frameTime=frameTime,
                .commandBuffer=commandBuffer,
                .camera=camera,
                .globalDescriptorSet=globalDescriptorSets[frameIndex],
                .frameDescriptorPool=*m_framePools[frameIndex],
                .objects=m_sceneManager.getObjects(),
                .lights=m_sceneManager.getLights()
            };
            ubo.projection=camera.getProjection();
            ubo.view=camera.getView();
            ubo.inverseView=camera.getInverseView();
            m_sceneManager.updateBuffer(ubo, frameIndex, frameTime);
            uboBuffers.at(frameIndex)->writeToBuffer(&ubo);
            uboBuffers.at(frameIndex)->flush();
            m_renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);
            objectRenderSystem.render(frameInfo);
            pointLightRenderSystem.render(frameInfo);

            if (m_gui.getState() != HIDDEN) {
                m_gui.render(
                    &m_renderer,
                    m_sceneManager,
                    camera,
                    m_device.getPhysicalDevice(),
                    ubo,
                    { .deltaTimeMS=clock.getDeltaTimeMS(), .fps=clock.getFPS() }
                    );
            }

            m_renderer.endSwapChainRenderPass(commandBuffer);
            m_renderer.endFrame();
            commandBuffer = nullptr;
        }
        if (m_sceneManager.getDestroyState()) {
            vkDeviceWaitIdle(m_device.device());
            m_sceneManager.destroyEntity(m_gui.getObjectsToRemove(), m_gui.getLightsToRemove());
        }
    }
    vkDeviceWaitIdle(m_device.device());
}

void ven::Engine::cleanup()
{
    Gui::cleanup();
}
