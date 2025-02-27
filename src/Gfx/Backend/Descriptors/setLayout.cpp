#include <array>

#include <vulkan/vulkan.h>

#include "VEngine/Gfx/Backend/Descriptors/SetLayout.hpp"
#include "Utils/ErrorHandling.hpp"

static VkDescriptorSetLayoutBinding binding(const uint32_t binding, const uint16_t descriptorCount, const VkDescriptorType descriptorType, const VkSampler *immutableSamplers, const VkShaderStageFlags stageFlags) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.pImmutableSamplers = immutableSamplers;
    layoutBinding.stageFlags = stageFlags;
    return layoutBinding;
}

void ven::DescriptorSetLayout::create(const uint16_t textureSize) {
    const std::array bindings = {
        binding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, VK_SHADER_STAGE_VERTEX_BIT),
        binding(1, textureSize, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, nullptr, VK_SHADER_STAGE_FRAGMENT_BIT)
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
        throw utl::THROW_ERROR("failed to create descriptor set layout!");
    }
}