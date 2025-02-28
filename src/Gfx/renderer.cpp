#include <glm/gtc/matrix_transform.hpp>

#include "VEngine/Gfx/Renderer.hpp"

void ven::Renderer::recordCommandBuffer(const uint32_t imageIndex, const uint32_t indiceSize, const VkDescriptorSet* descriptorSet, const VkCommandBuffer& commandBuffer, const VkBuffer& indexBuffer, const VkBuffer& vertexBuffer) {
    const VkExtent2D extent = m_swapChain.getExtent();
    const VkViewport viewport{.x=0.0F, .y=0.0F, .width=static_cast<float>(extent.width), .height=static_cast<float>(extent.height), .minDepth=0.0F, .maxDepth=1.0F };
    const VkRect2D scissor{ .offset = { .x=0, .y=0 }, .extent = extent };
    constexpr VkCommandBufferBeginInfo beginInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to begin recording command buffer!");
    }
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_swapChain.getRenderPass();
    renderPassInfo.framebuffer = m_swapChain.getSwapChainFrameBuffers().at(imageIndex);
    renderPassInfo.renderArea.offset = { .x=0, .y=0 };
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(m_clearValues.size());
    renderPassInfo.pClearValues = m_clearValues.data();
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_shadersModule.getPipeline());
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    const std::array vertexBuffers = {vertexBuffer};
    constexpr std::array<VkDeviceSize, 1> offsets = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers.data(), offsets.data());
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_shadersModule.getPipelineLayout(), 0, 1, descriptorSet, 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, indiceSize, 1, 0, 0, 0);
    m_gui.render(commandBuffer);
    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to record command buffer!");
    }
}

void ven::Renderer::createCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers) const {
    commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_device.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    if (vkAllocateCommandBuffers(m_device.getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to allocate command buffers!");
    }
}

void ven::Renderer::recreateSwapChain() {
    int width = 0;
    int height = 0;
    m_window.getFrameBufferSize(width, height);
    while (width == 0 || height == 0) {
        m_window.getFrameBufferSize(width, height);
        Window::waitEvents();
    }
    m_device.waitIdle();
    m_swapChain.cleanupSwapChain();
    m_swapChain.init();
}

void ven::Renderer::updateUniformBuffer(void* uniformBufferMapped, std::vector<Model>& models) const {
    for (size_t i = 0; i < models.size(); ++i) {
        Model& model = models[i];
        UniformBufferObject ubo{};
        auto modelmat = glm::mat4(1.0F);

        modelmat = glm::rotate(modelmat, model.getTransform().rotation.x, glm::vec3(1.0F, 0.0F, 0.0F));
        modelmat = glm::rotate(modelmat, model.getTransform().rotation.y, glm::vec3(0.0F, 1.0F, 0.0F));
        modelmat = glm::rotate(modelmat, model.getTransform().rotation.z, glm::vec3(0.0F, 0.0F, 1.0F));
        modelmat = glm::translate(modelmat, model.getTransform().position);
        modelmat = glm::scale(modelmat, model.getTransform().scale);

        ubo.model = modelmat;
        ubo.view = m_camera.getViewMatrix();
        ubo.proj = m_camera.getProjectionMatrix(static_cast<float>(m_swapChain.getExtent().width) / static_cast<float>(m_swapChain.getExtent().height));
        ubo.proj[1][1] *= -1;
        ubo.ambientColor = m_ambientColor;
        memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
    }
}


ven::Renderer::~Renderer() = default;
