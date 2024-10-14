#include <cassert>

#include "VEngine/Descriptors/DescriptorSetLayout.hpp"

ven::DescriptorSetLayout::Builder &ven::DescriptorSetLayout::Builder::addBinding(const uint32_t binding, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, const uint32_t count)
{
    assert(m_bindings.contains(binding) == 0 && "Binding already exists in layout");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    m_bindings[binding] = layoutBinding;
    return *this;
}

ven::DescriptorSetLayout::DescriptorSetLayout(Device &device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings) : m_device{device}, m_bindings{bindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    setLayoutBindings.reserve(bindings.size());
    for (auto [fst, snd] : bindings) {
        setLayoutBindings.push_back(snd);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(
            m_device.device(),
            &descriptorSetLayoutInfo,
            nullptr,
            &m_descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}
