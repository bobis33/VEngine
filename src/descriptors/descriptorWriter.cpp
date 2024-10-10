#include <cassert>
#include <functional>

#include "VEngine/Descriptors/DescriptorWriter.hpp"

ven::DescriptorWriter &ven::DescriptorWriter::writeBuffer(const uint32_t binding, const VkDescriptorBufferInfo *bufferInfo)
{
    assert(m_setLayout.m_bindings.count(binding) == 1 && "Layout does not contain specified binding");

    const auto &bindingDescription = m_setLayout.m_bindings.at(binding);

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    m_writes.push_back(write);
    return *this;
}

ven::DescriptorWriter &ven::DescriptorWriter::writeImage(const uint32_t binding, const VkDescriptorImageInfo *imageInfo)
{
    assert(m_setLayout.m_bindings.count(binding) == 1 && "Layout does not contain specified binding");

    const VkDescriptorSetLayoutBinding &bindingDescription = m_setLayout.m_bindings.at(binding);

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    m_writes.push_back(write);
    return *this;
}

bool ven::DescriptorWriter::build(VkDescriptorSet &set)
{
    if (!m_pool.allocateDescriptor(m_setLayout.getDescriptorSetLayout(), set)) {
        return false;
    }
    overwrite(set);
    return true;
}

void ven::DescriptorWriter::overwrite(const VkDescriptorSet &set) {
    for (auto &[sType, pNext, dstSet, dstBinding, dstArrayElement, descriptorCount, descriptorType, pImageInfo, pBufferInfo, pTexelBufferView] : m_writes) {
        dstSet = set;
    }
    vkUpdateDescriptorSets(m_pool.m_device.device(), static_cast<unsigned int>(m_writes.size()), m_writes.data(), 0, nullptr);
}
