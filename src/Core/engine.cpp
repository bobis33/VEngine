#include "VEngine/Core/Engine.hpp"
#include "VEngine/Core/EventManager.hpp"
#include "VEngine/Core/RenderSystem/PointLight.hpp"
#include "VEngine/Gfx/Descriptors/Writer.hpp"
#include "VEngine/Factories/Light.hpp"
#include "VEngine/Factories/Object.hpp"
#include "VEngine/Factories/Model.hpp"
#include "VEngine/Utils/Clock.hpp"
#include "VEngine/Utils/Colors.hpp"
#include "VEngine/Utils/Logger.hpp"

ven::Engine::Engine(const Config& config) : m_state(EDITOR), m_window(config.window.width, config.window.height), m_camera(config.camera.fov, config.camera.near, config.camera.far, config.camera.move_speed, config.camera.look_speed) {
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
    constexpr std::array lightColors{Colors::RED_4, Colors::GREEN_4, Colors::BLUE_4, Colors::YELLOW_4, Colors::CYAN_4, Colors::MAGENTA_4};

    Logger::logExecutionTime("Creating object sponza", [&] {
        m_sceneManager.addObject(ObjectFactory::create(
            nullptr,
            ModelFactory::get(m_device, "assets/models/sponzaObj/sponza.obj"),
            "sponza",
            {
            .translation = {0.F, 0.F, 0.F},
            .scale = {0.01F, 0.01F, 0.01F},
            .rotation = {0.F, 0.F, -3.14159265358979323846264338327950288419716939937510582F} // == -π, why ?
        }));
    });
    for (std::size_t i = 0; i < lightColors.size(); i++)
    {
        Logger::logExecutionTime("Creating light n" + std::to_string(i), [&] {
            const glm::mat4 rotateLight = rotate(
                glm::mat4(1.F),
                static_cast<float>(i) * glm::two_pi<float>() / 6.0F, // 6 = num of lights
                {0.F, -1.F, 0.F}
);
            m_sceneManager.addLight(LightFactory::create({
                    .translation = glm::vec3(rotateLight * glm::vec4(-1.F, -1.F, -1.F, 1.F)),
                    .scale = { 0.1F, 0.0F, 0.0F },
                    .rotation = { 0.F, 0.F, 0.F }},
                lightColors.at(i)
                ));
        });
    }
}

void ven::Engine::run()
{
    int a = 0;
    Clock clock;
    const EventManager eventManager{};
    GlobalUbo ubo{};
    VkCommandBuffer_T *commandBuffer = nullptr;
    VkDescriptorBufferInfo bufferInfo{};
    float frameTime = 0.0F;
    unsigned long frameIndex = 0;
    std::vector<std::unique_ptr<Buffer>> uboBuffers(MAX_FRAMES_IN_FLIGHT);
    std::vector<VkDescriptorSet> globalDescriptorSets(MAX_FRAMES_IN_FLIGHT);
    const PointLightRenderSystem pointLightRenderSystem(m_device, m_renderer.getSwapChainRenderPass(), m_globalSetLayout->getDescriptorSetLayout());

    for (auto& uboBuffer : uboBuffers)
    {
        uboBuffer = std::make_unique<Buffer>(m_device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffer->map();
    }
    for (std::size_t i = 0; i < globalDescriptorSets.size(); i++) {
        bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*m_globalSetLayout, *m_globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    while (m_state != EXIT)
    {
        clock.update();
        frameTime = clock.getDeltaTime();
        eventManager.handleEvents(m_window.getGLFWindow(), &m_state, m_camera, m_gui, frameTime);
        commandBuffer = m_renderer.beginFrame();

        m_camera.setViewXYZ(m_camera.transform.translation, m_camera.transform.rotation);
        m_camera.setPerspectiveProjection(m_renderer.getAspectRatio());

        if (commandBuffer != nullptr) {
            frameIndex = m_renderer.getFrameIndex();
            m_framePools[frameIndex]->resetPool();
            FrameInfo frameInfo{
                .frameIndex=frameIndex,
                .frameTime=frameTime,
                .commandBuffer=commandBuffer,
                .camera=m_camera,
                .globalDescriptorSet=globalDescriptorSets[frameIndex],
                .frameDescriptorPool=*m_framePools[frameIndex],
                .objects=m_sceneManager.getObjects(),
                .lights=m_sceneManager.getLights()
            };
            ubo.projection=m_camera.getProjection();
            ubo.view=m_camera.getView();
            ubo.inverseView=m_camera.getInverseView();
            m_sceneManager.updateBuffer(ubo, frameIndex, frameTime);
            uboBuffers.at(frameIndex)->writeToBuffer(&ubo);
            uboBuffers.at(frameIndex)->flush();
            m_renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);
            m_objectRenderSystem.render(frameInfo);
            pointLightRenderSystem.render(frameInfo);

            if (m_gui.getState() != HIDDEN) {
                m_gui.render(
                    &m_renderer,
                    m_sceneManager,
                    m_camera,
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
