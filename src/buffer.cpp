#include <cassert>
#include <cstring>

#include "VEngine/Buffer.hpp"

VkDeviceSize ven::Buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
    if (minOffsetAlignment > 0) {
        return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    }
    return instanceSize;
}

ven::Buffer::Buffer(ven::Device &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : m_device{device}, m_instanceSize{instanceSize}, m_instanceCount{instanceCount}, m_alignmentSize(getAlignment(instanceSize, minOffsetAlignment)), m_usageFlags{usageFlags}, m_memoryPropertyFlags{memoryPropertyFlags}
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

VkResult ven::Buffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    assert(m_buffer && m_memory && "Called map on m_buffer before create");
    return vkMapMemory(m_device.device(), m_memory, offset, size, 0, &m_mapped);
}

void ven::Buffer::unmap()
{
    if (m_mapped != nullptr) {
        vkUnmapMemory(m_device.device(), m_memory);
        m_mapped = nullptr;
    }
}

void ven::Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
{
    assert(m_mapped && "Cannot copy to unmapped m_buffer");

    if (size == VK_WHOLE_SIZE) {
        memcpy(m_mapped, data, m_bufferSize);
    } else {
        char *memOffset = static_cast<char *>(m_mapped);
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult ven::Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = m_memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

VkResult ven::Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = m_memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkInvalidateMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}
