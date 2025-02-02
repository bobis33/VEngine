#include "VEngine/Core/RenderSystem/ABase.hpp"

void ven::ARenderSystemBase::createPipelineLayout(const VkDescriptorSetLayout globalSetLayout, const uint32_t pushConstantSize)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = pushConstantSize;

    renderSystemLayout =
    DescriptorSetLayout::Builder(m_device)
        .addBinding(
            0,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
        .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        .build();

    const std::vector<VkDescriptorSetLayout> descriptorSetLayouts{
        globalSetLayout,
        renderSystemLayout->getDescriptorSetLayout()
    };

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

void ven::ARenderSystemBase::createPipeline(const VkRenderPass renderPass, const std::string &shadersVertPath, const std::string &shadersFragPath, const bool isLight)
{
    assert(m_pipelineLayout && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfig{};
    Shaders::defaultPipelineConfigInfo(pipelineConfig);
    if (isLight) {
        pipelineConfig.attributeDescriptions.clear();
    	pipelineConfig.bindingDescriptions.clear();
    }
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_shaders = std::make_unique<Shaders>(m_device, shadersVertPath, shadersFragPath, pipelineConfig);
}
