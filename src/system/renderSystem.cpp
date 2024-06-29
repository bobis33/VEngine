#include "VEngine/System/RenderSystem.hpp"

ven::RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout) : m_device{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

ven::RenderSystem::~RenderSystem()
{
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void ven::RenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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
    m_shaders = std::make_unique<Shaders>(m_device, std::string(SHADERS_BIN_PATH) + "shader_vert.spv", std::string(SHADERS_BIN_PATH) + "shader_frag.spv", pipelineConfig);
}

void ven::RenderSystem::renderObjects(FrameInfo &frameInfo)
{
    m_shaders->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto &kv : frameInfo.objects)
    {
        Object &object = kv.second;
        if (object.model == nullptr) continue;
        SimplePushConstantData push{};
        push.modelMatrix = object.transform3D.mat4();
        push.normalMatrix = object.transform3D.normalMatrix();
        vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
        object.model->bind(frameInfo.commandBuffer);
        object.model->draw(frameInfo.commandBuffer);
    }
}
