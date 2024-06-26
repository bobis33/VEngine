#include <cassert>

#include "VEngine/Descriptors.hpp"

ven::DescriptorSetLayout::Builder &ven::DescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
    assert(m_bindings.count(binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    m_bindings[binding] = layoutBinding;
    return *this;
}

std::unique_ptr<ven::DescriptorSetLayout> ven::DescriptorSetLayout::Builder::build() const
{
    return std::make_unique<ven::DescriptorSetLayout>(m_device, m_bindings);
}

ven::DescriptorSetLayout::DescriptorSetLayout(ven::Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) : m_device{device}, m_bindings{bindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto kv : bindings) {
        setLayoutBindings.push_back(kv.second);
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

ven::DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
}


ven::DescriptorPool::Builder &ven::DescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    m_poolSizes.push_back({descriptorType, count});
    return *this;
}

ven::DescriptorPool::Builder &ven::DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
{
    m_poolFlags = flags;
    return *this;
}
ven::DescriptorPool::Builder &ven::DescriptorPool::Builder::setMaxSets(uint32_t count)
{
    m_maxSets = count;
    return *this;
}

std::unique_ptr<ven::DescriptorPool> ven::DescriptorPool::Builder::build() const
{
    return std::make_unique<ven::DescriptorPool>(m_device, m_maxSets, m_poolFlags, m_poolSizes);
}

ven::DescriptorPool::DescriptorPool(Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes) : m_device{device}
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(m_device.device(), &descriptorPoolInfo, nullptr, &m_descriptorPool) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

ven::DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr);
}

bool ven::DescriptorPool::allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const
        {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool whenever an old pool fills up. But this is beyond our current scope
    if (vkAllocateDescriptorSets(m_device.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}

void ven::DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const
{
    vkFreeDescriptorSets(
            m_device.device(),
            m_descriptorPool,
            static_cast<uint32_t>(descriptors.size()),
            descriptors.data());
}

void ven::DescriptorPool::resetPool()
{
    vkResetDescriptorPool(m_device.device(), m_descriptorPool, 0);
}

ven::DescriptorWriter::DescriptorWriter(ven::DescriptorSetLayout &setLayout, ven::DescriptorPool &pool) : m_setLayout{setLayout}, m_pool{pool} {}

ven::DescriptorWriter &ven::DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = m_setLayout.m_bindings[binding];

    assert(
            bindingDescription.descriptorCount == 1 &&
            "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    m_writes.push_back(write);
    return *this;
}

ven::DescriptorWriter &ven::DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = m_setLayout.m_bindings[binding];

    assert(
            bindingDescription.descriptorCount == 1 &&
            "Binding single descriptor info, but binding expects multiple");

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
    bool success = m_pool.allocateDescriptor(m_setLayout.getDescriptorSetLayout(), set);
    if (!success) {
        return false;
    }
    overwrite(set);
    return true;
}

void ven::DescriptorWriter::overwrite(VkDescriptorSet &set)
{
    for (auto &write : m_writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(m_pool.m_device.device(), static_cast<unsigned int>(m_writes.size()), m_writes.data(), 0, nullptr);
}
