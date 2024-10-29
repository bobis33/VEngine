#include <ranges>

#include "VEngine/Core/RenderSystem/PointLight.hpp"

void ven::PointLightRenderSystem::render(const FrameInfo &frameInfo) const
{
    getShaders()->bind(frameInfo.commandBuffer);
    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (const Light &light : frameInfo.lights | std::views::values) {
        const LightPushConstantData push{
            .position = glm::vec4(light.transform.translation, 1.F),
            .color = light.color,
            .radius = light.transform.scale.x
        };
        vkCmdPushConstants(frameInfo.commandBuffer, getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(LightPushConstantData), &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}
