#include <ranges>

#include "VEngine/RenderSystem/PointLightRenderSystem.hpp"

void ven::PointLightRenderSystem::render(const FrameInfo &frameInfo) const
{
    getShaders()->bind(frameInfo.commandBuffer);
    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (const Light &light : frameInfo.lights | std::views::values) {
        const LightPushConstantData push{
            .position = glm::vec4(light.transform3D.translation, 1.F),
            .color = light.color,
            .radius = light.transform3D.scale.x
        };
        vkCmdPushConstants(frameInfo.commandBuffer, getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(LightPushConstantData), &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}

void ven::PointLightRenderSystem::update(const FrameInfo &frameInfo, GlobalUbo &ubo)
{
    const glm::mat4 rotateLight = rotate(glm::mat4(1.F), frameInfo.frameTime, {0.F, -1.F, 0.F});
    uint16_t lightIndex = 0;

    for (Light &light : frameInfo.lights | std::views::values) {
        assert(lightIndex < MAX_LIGHTS && "Too many lights");
        light.transform3D.translation = glm::vec3(rotateLight * glm::vec4(light.transform3D.translation, 1.F));
        ubo.pointLights.at(lightIndex).position = glm::vec4(light.transform3D.translation, 1.F);
        ubo.pointLights.at(lightIndex).color = light.color;
        lightIndex++;
    }
    ubo.numLights = lightIndex;
}
