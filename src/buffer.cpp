#include <cassert>
#include <cstring>

#include "VEngine/Buffer.hpp"

VkDeviceSize ven::Buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
    if (minOffsetAlignment > 0) {
        return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    }
    return instanceSize;
}

ven::Buffer::Buffer(ven::Device &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : lveDevice{device}, instanceSize{instanceSize}, instanceCount{instanceCount}, usageFlags{usageFlags}, memoryPropertyFlags{memoryPropertyFlags}
{
    alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
    bufferSize = alignmentSize * instanceCount;
    device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

ven::Buffer::~Buffer()
{
    unmap();
    vkDestroyBuffer(lveDevice.device(), buffer, nullptr);
    vkFreeMemory(lveDevice.device(), memory, nullptr);
}

VkResult ven::Buffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    assert(buffer && memory && "Called map on buffer before create");
    return vkMapMemory(lveDevice.device(), memory, offset, size, 0, &mapped);
}

void ven::Buffer::unmap()
{
    if (mapped) {
        vkUnmapMemory(lveDevice.device(), memory);
        mapped = nullptr;
    }
}

void ven::Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
{
    assert(mapped && "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE) {
        memcpy(mapped, data, bufferSize);
    } else {
        char *memOffset = (char *)mapped;
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult ven::Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(lveDevice.device(), 1, &mappedRange);
}

VkResult ven::Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkInvalidateMappedMemoryRanges(lveDevice.device(), 1, &mappedRange);
}
