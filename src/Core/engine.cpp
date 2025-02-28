#include "Utils/Logger.hpp"
#include "VEngine/Core/Engine.hpp"

void ven::Engine::init() {
    m_descriptorSetLayout.create(TextureManager::getTextureSize());
    m_renderer.getShadersModule().createPipeline(m_device.getMsaaSamples(), m_descriptorSetLayout.getDescriptorSetLayout(), m_renderer.getSwapChain().getRenderPass());
    createUniformBuffers();
    m_descriptorSets.create(Renderer::UNIFORM_BUFFER_SIZE);
    m_renderer.createCommandBuffers(m_commandBuffers);
}

void ven::Engine::loadAssets() {
    std::vector<std::string> modelPaths = {"assets/models/sponza/sponza.obj", "assets/models/book.obj", "assets/models/viking_room.obj"};
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vertexOffset = 0;
    TextureManager::loadTextures(m_device, m_renderer.getSwapChain(), "assets/textures");
    for (const auto& path : modelPaths) {
        utl::Logger::logExecutionTime("Loading model: " + path, [&] { m_models.emplace_back(m_device, m_renderer.getSwapChain(), path); });
        for (Model& model = m_models.back(); const auto& mesh : model.getMeshes()) {
            for (const auto& vertex : mesh->getVertices()) {
                vertices.push_back(vertex);
            }
            for (const auto& index : mesh->getIndices()) {
                indices.push_back(index + vertexOffset);
            }
            vertexOffset += static_cast<uint32_t>(mesh->getVertices().size());
        }
    }
    utl::Logger::logInfo("Textures loaded: " + std::to_string(TextureManager::getTextureSize()));
    m_indicesSize = static_cast<uint32_t>(indices.size());
    Model::createBuffer(m_device, vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_vertexBuffer, m_vertexBufferMemory);
    Model::createBuffer(m_device, indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, m_indexBuffer, m_indexBufferMemory);
}

void ven::Engine::run() {
    while (!m_window.shouldClose()) {
        m_eventManager.handleEvents(m_clock.getDeltaSeconds());
        m_clock.restart();
        drawFrame();
    }
    m_device.waitIdle();
}

void ven::Engine::createUniformBuffers() {
    m_uniformBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    m_uniformBuffersMemory.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    m_uniformBuffersMapped.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (uint8_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        m_device.createBuffer(Renderer::UNIFORM_BUFFER_SIZE, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers.at(i), m_uniformBuffersMemory.at(i));
        vkMapMemory(m_device.getVkDevice(), m_uniformBuffersMemory.at(i), 0, Renderer::UNIFORM_BUFFER_SIZE, 0, &m_uniformBuffersMapped.at(i));
    }
}

void ven::Engine::drawFrame() {
    uint32_t imageIndex = 0;
    if (vkWaitForFences(m_device.getVkDevice(), 1, &m_renderer.getSwapChain().getInFlightFences().at(m_currentFrame), VK_TRUE, UINT64_MAX) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to wait for fence!");
    }
    VkResult result = m_renderer.getSwapChain().acquireNextImage(imageIndex, m_currentFrame);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_renderer.recreateSwapChain();
        return;
    } if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw utl::THROW_ERROR("failed to acquire swap chain image!");
    }
    m_renderer.updateUniformBuffer(m_uniformBuffersMapped.at(m_currentFrame), m_models);
    vkResetFences(m_device.getVkDevice(), 1, &m_renderer.getSwapChain().getInFlightFences().at(m_currentFrame));
    vkResetCommandBuffer(m_commandBuffers.at(m_currentFrame), /*VkCommandBufferResetFlagBits*/ 0);
    m_renderer.recordCommandBuffer(imageIndex, m_indicesSize, &m_descriptorSets.getDescriptorSets().at(m_currentFrame), m_commandBuffers.at(m_currentFrame), m_indexBuffer, m_vertexBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    const std::array waitSemaphores = {m_renderer.getSwapChain().getImageAvailableSemaphores().at(m_currentFrame)};
    constexpr std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers.at(m_currentFrame);
    const std::array signalSemaphores = {m_renderer.getSwapChain().getRenderFinishedSemaphores().at(m_currentFrame)};
    submitInfo.signalSemaphoreCount = signalSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphores.data();
    if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, m_renderer.getSwapChain().getInFlightFences().at(m_currentFrame)) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to submit draw command buffer!");
    }
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = signalSemaphores.size();
    presentInfo.pWaitSemaphores = signalSemaphores.data();
    const std::array swapChains = {m_renderer.getSwapChain().getSwapChain()};
    presentInfo.swapchainCount = swapChains.size();
    presentInfo.pSwapchains = swapChains.data();
    presentInfo.pImageIndices = &imageIndex;
    result = vkQueuePresentKHR(m_device.getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasWindowResized()) {
        m_window.resetWindowResizedFlag();
        m_renderer.recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to present swap chain image!");
    }
    m_currentFrame = (m_currentFrame + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}
