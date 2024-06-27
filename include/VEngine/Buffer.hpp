/*
** @file Buffer.hpp
** @brief This file contains the Buffer class
** @namespace ven
*/

#pragma once

#include "VEngine/Device.hpp"

namespace ven {

    class Buffer {
    public:
        Buffer(Device& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        /**
         * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
         *
         * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
         * buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the buffer mapping call
         */
        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Unmap a mapped memory range
         *
         * @note Does not return a result as vkUnmapMemory can't fail
         */
        void unmap();

        /**
         * Copies the specified data to the mapped buffer. Default value writes whole buffer range
         *
         * @param data Pointer to the data to copy
         * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
         * range.
         * @param offset (Optional) Byte offset from beginning of mapped region
         *
         */
        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Flush a memory range of the buffer to make it visible to the device
         *
         * @note Only required for non-coherent memory
         *
         * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
         * complete buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the flush call
         */
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Create a buffer info descriptor
         *
         * @param size (Optional) Size of the memory range of the descriptor
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkDescriptorBufferInfo of specified offset and range
         */
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) { return VkDescriptorBufferInfo{buffer, offset, size, }; }

        /**
         * Invalidate a memory range of the buffer to make it visible to the host
         *
         * @note Only required for non-coherent memory
         *
         * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
         * the complete buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the invalidate call
         */
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
         *
         * @param data Pointer to the data to copy
         * @param index Used in offset calculation
         *
         */
        void writeToIndex(void* data, int index) { writeToBuffer(data, instanceSize, index * alignmentSize); }

        /**
         *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
         *
         * @param index Used in offset calculation
         *
         */
        VkResult flushIndex(int index) { return flush(alignmentSize, index * alignmentSize); }

        /**
         * Create a buffer info descriptor
         *
         * @param index Specifies the region given by index * alignmentSize
         *
         * @return VkDescriptorBufferInfo for instance at index
         */
        VkDescriptorBufferInfo descriptorInfoForIndex(int index) { return descriptorInfo(alignmentSize, index * alignmentSize); }

        /**
         * Invalidate a memory range of the buffer to make it visible to the host
         *
         * @note Only required for non-coherent memory
         *
         * @param index Specifies the region to invalidate: index * alignmentSize
         *
         * @return VkResult of the invalidate call
         */
        VkResult invalidateIndex(int index) { return invalidate(alignmentSize, index * alignmentSize); }

        VkBuffer getBuffer() const { return buffer; }
        void* getMappedMemory() const { return mapped; }
        uint32_t getInstanceCount() const { return instanceCount; }
        VkDeviceSize getInstanceSize() const { return instanceSize; }
        VkDeviceSize getAlignmentSize() const { return instanceSize; }
        VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
        VkDeviceSize getBufferSize() const { return bufferSize; }

    private:
        /**
         * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
         *
         * @param instanceSize The size of an instance
         * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
         * minUniformBufferOffsetAlignment)
         *
         * @return VkResult of the buffer mapping call
         */
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        Device& lveDevice;
        void* mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;

    }; // class Buffer

} // namespace ven
