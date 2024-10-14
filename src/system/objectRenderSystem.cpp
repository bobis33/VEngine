#include <ranges>

#include "VEngine/RenderSystem/ObjectRenderSystem.hpp"

void ven::ObjectRenderSystem::render(const FrameInfo &frameInfo) const
{
    getShaders()->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (const Object& object : frameInfo.objects | std::views::values) {
        if (object.getModel() == nullptr) { continue; }
        const ObjectPushConstantData push{
            .modelMatrix = object.transform3D.mat4(),
            .normalMatrix = object.transform3D.normalMatrix()
        };
        vkCmdPushConstants(frameInfo.commandBuffer, getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ObjectPushConstantData), &push);
        object.getModel()->bind(frameInfo.commandBuffer);
        object.getModel()->draw(frameInfo.commandBuffer);
    }
}
