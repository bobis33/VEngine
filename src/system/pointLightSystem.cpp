#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VEngine/System/PointLightSystem.hpp"

struct PointLightPushConstants {
    glm::vec4 position{};
    glm::vec4 color{};
    float radius;
};

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
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

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

    for (auto &kv : frameInfo.objects)
    {
        Object &object = kv.second;
        if (object.pointLight == nullptr) continue;
        PointLightPushConstants push{};
        push.position = glm::vec4(object.transform3D.translation, 1.F);
        push.color = glm::vec4(object.color, object.pointLight->lightIntensity);
        push.radius = object.transform3D.scale.x;
        vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }

}

void ven::PointLightSystem::update(ven::FrameInfo &frameInfo, ven::GlobalUbo &ubo)
{
    auto rotateLight = glm::rotate(glm::mat4(1.F), frameInfo.frameTime, {0.F, -1.F, 0.F});
    unsigned long lightIndex = 0;
    for (auto &kv : frameInfo.objects)
    {
        Object &object = kv.second;
        if (object.pointLight == nullptr) continue;
        assert(lightIndex < MAX_LIGHTS && "Too many lights");
        object.transform3D.translation = glm::vec3(rotateLight * glm::vec4(object.transform3D.translation, 1.F));
        ubo.pointLights[lightIndex].position = glm::vec4(object.transform3D.translation, 1.F);
        ubo.pointLights[lightIndex].color = glm::vec4(object.color, object.pointLight->lightIntensity);
        lightIndex++;
    }
    ubo.numLights = static_cast<int>(lightIndex);
}
