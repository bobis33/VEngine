#include <iostream>
#include <memory>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "VEngine/RenderSystem.hpp"
#include "VEngine/Engine.hpp"

ven::RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass) : m_device{device}
{
    createPipelineLayout();
    createPipeline(renderPass);
}

ven::RenderSystem::~RenderSystem()
{
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void ven::RenderSystem::createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void ven::RenderSystem::createPipeline(VkRenderPass renderPass)
{
    PipelineConfigInfo pipelineConfig{};
    ven::Shaders::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_shaders = std::make_unique<Shaders>(m_device, "shaders/bin/vertex.spv", "shaders/bin/fragment.spv", pipelineConfig);
}

void ven::RenderSystem::renderObjects(VkCommandBuffer commandBuffer, std::vector<Object> &gameObjects)
{
    m_shaders->bind(commandBuffer);
    for (auto &object : gameObjects)
    {
        object.transform2d.rotation = glm::mod(object.transform2d.rotation + 0.001F, glm::two_pi<float>());
        SimplePushConstantData push{};
        push.offset = object.transform2d.translation;
        push.color = object.color;
        push.transform = object.transform2d.mat2();

        vkCmdPushConstants(commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
        object.model->bind(commandBuffer);
        object.model->draw(commandBuffer);

    }
}
