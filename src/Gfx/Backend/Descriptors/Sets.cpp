#include <vector>

#include <vulkan/vulkan_core.h>

#include "VEngine/Gfx/Backend/Descriptors/Sets.hpp"
#include "VEngine/Gfx/Resources/TextureManager.hpp"

void ven::DescriptorSets::create(const VkDeviceSize bufferSize) {
    VkDescriptorSetAllocateInfo allocInfo{};
    const std::vector layouts(SwapChain::MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChain::MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();
    m_descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(m_device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to allocate descriptor sets!");
    }
    for (uint8_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_buffers.at(i);
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        VkWriteDescriptorSet uboWrite{};
        uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        uboWrite.dstSet = m_descriptorSets.at(i);
        uboWrite.dstBinding = 0;
        uboWrite.dstArrayElement = 0;
        uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboWrite.descriptorCount = 1;
        uboWrite.pBufferInfo = &bufferInfo;
        descriptorWrites.push_back(uboWrite);
        std::vector<VkDescriptorImageInfo> imageInfos;
        for (uint8_t textureIndex = 0; textureIndex < TextureManager::getTextureSize(); textureIndex++) {
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = TextureManager::getTexture(textureIndex)->getTextureImageView();
            imageInfo.sampler = TextureManager::getTexture(textureIndex)->getTextureSampler();
            imageInfos.push_back(imageInfo);
        }
        VkWriteDescriptorSet samplerWrite{};
        samplerWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        samplerWrite.dstSet = m_descriptorSets.at(i);
        samplerWrite.dstBinding = 1;
        samplerWrite.dstArrayElement = 0;
        samplerWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerWrite.descriptorCount = static_cast<uint32_t>(imageInfos.size());
        samplerWrite.pImageInfo = imageInfos.data();
        descriptorWrites.push_back(samplerWrite);
        vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}
