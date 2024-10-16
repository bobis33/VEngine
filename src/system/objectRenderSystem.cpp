#include <ranges>

#include <VEngine/Descriptors/DescriptorWriter.hpp>
#include "VEngine/RenderSystem/ObjectRenderSystem.hpp"

void ven::ObjectRenderSystem::render(const FrameInfo &frameInfo) const
{
    getShaders()->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (Object& object : frameInfo.objects | std::views::values) {
        if (object.getModel() == nullptr) { continue; }
        auto bufferInfo = object.getBufferInfo(static_cast<int>(frameInfo.frameIndex));
        auto imageInfo = object.getDiffuseMap()->getImageInfo();
        VkDescriptorSet objectDescriptorSet = nullptr;
        DescriptorWriter(*renderSystemLayout, frameInfo.frameDescriptorPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &imageInfo)
            .build(objectDescriptorSet);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            getPipelineLayout(),
            1,  // starting set (0 is the globalDescriptorSet, 1 is the set specific to this system)
            1,  // set count
            &objectDescriptorSet,
            0,
            nullptr);

        const ObjectPushConstantData push{
            .modelMatrix = object.transform.mat4(),
            .normalMatrix = object.transform.normalMatrix()
        };
        vkCmdPushConstants(frameInfo.commandBuffer, getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ObjectPushConstantData), &push);
        object.getModel()->bind(frameInfo.commandBuffer);
        object.getModel()->draw(frameInfo.commandBuffer);
    }
}
