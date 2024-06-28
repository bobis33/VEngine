#include <array>
#include <cassert>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VEngine/System/PointLightSystem.hpp"

ven::PointLightSystem::PointLightSystem(Device& device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout) : m_device{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

ven::PointLightSystem::~PointLightSystem()
{
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void ven::PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    /*
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
    */

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void ven::PointLightSystem::createPipeline(VkRenderPass renderPass)
{
    PipelineConfigInfo pipelineConfig{};
    ven::Shaders::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_shaders = std::make_unique<Shaders>(m_device, std::string(SHADERS_BIN_PATH) + "point_light_vert.spv", std::string(SHADERS_BIN_PATH) + "point_light_frag.spv", pipelineConfig);
}

void ven::PointLightSystem::render(FrameInfo &frameInfo)
{
    m_shaders->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}
