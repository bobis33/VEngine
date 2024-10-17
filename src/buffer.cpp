#include <cassert>
#include <cstring>

#include "VEngine/Buffer.hpp"

ven::Buffer::Buffer(Device &device, const VkDeviceSize instanceSize, const uint32_t instanceCount, const VkBufferUsageFlags usageFlags, const VkMemoryPropertyFlags memoryPropertyFlags, const VkDeviceSize minOffsetAlignment) : m_device{device}, m_instanceSize{instanceSize}, m_instanceCount{instanceCount}, m_alignmentSize(getAlignment(instanceSize, minOffsetAlignment)), m_usageFlags{usageFlags}, m_memoryPropertyFlags{memoryPropertyFlags}
{
    m_bufferSize = m_alignmentSize * m_instanceCount;
    device.createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
}

ven::Buffer::~Buffer()
{
    unmap();
    vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
    vkFreeMemory(m_device.device(), m_memory, nullptr);
}

VkResult ven::Buffer::map(const VkDeviceSize size, const VkDeviceSize offset)
{
    assert(m_buffer && m_memory && "Called map on buffer before create");
    return vkMapMemory(m_device.device(), m_memory, offset, size, 0, &m_mapped);
}

void ven::Buffer::unmap()
{
    if (m_mapped != nullptr) {
        vkUnmapMemory(m_device.device(), m_memory);
        m_mapped = nullptr;
    }
}

void ven::Buffer::writeToBuffer(const void *data, const VkDeviceSize size, const VkDeviceSize offset) const
{
    assert(m_mapped && "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE) {
        memcpy(m_mapped, data, m_bufferSize);
    } else {
        auto memOffset = static_cast<char *>(m_mapped);
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult ven::Buffer::flush(const VkDeviceSize size, const VkDeviceSize offset) const
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = m_memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

VkResult ven::Buffer::invalidate(const VkDeviceSize size, const VkDeviceSize offset) const
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = m_memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkInvalidateMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}
