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
         * Map a m_memory range of this m_buffer. If successful, m_mapped points to the specified m_buffer range.
         *
         * @param size (Optional) Size of the m_memory range to map. Pass VK_WHOLE_SIZE to map the complete
         * m_buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the m_buffer mapping call
         */
        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Unmap a m_mapped m_memory range
         *
         * @note Does not return a result as vkUnmapMemory can't fail
         */
        void unmap();

        /**
         * Copies the specified data to the m_mapped m_buffer. Default value writes whole m_buffer range
         *
         * @param data Pointer to the data to copy
         * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete m_buffer
         * range.
         * @param offset (Optional) Byte offset from beginning of m_mapped region
         *
         */
        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Flush a m_memory range of the m_buffer to make it visible to the device
         *
         * @note Only required for non-coherent m_memory
         *
         * @param size (Optional) Size of the m_memory range to flush. Pass VK_WHOLE_SIZE to flush the
         * complete m_buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the flush call
         */
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Create a m_buffer info descriptor
         *
         * @param size (Optional) Size of the m_memory range of the descriptor
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkDescriptorBufferInfo of specified offset and range
         */
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) { return VkDescriptorBufferInfo{m_buffer, offset, size, }; }

        /**
         * Invalidate a m_memory range of the m_buffer to make it visible to the host
         *
         * @note Only required for non-coherent m_memory
         *
         * @param size (Optional) Size of the m_memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
         * the complete m_buffer range.
         * @param offset (Optional) Byte offset from beginning
         *
         * @return VkResult of the invalidate call
         */
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /**
         * Copies "m_instanceSize" bytes of data to the m_mapped m_buffer at an offset of index * m_alignmentSize
         *
         * @param data Pointer to the data to copy
         * @param index Used in offset calculation
         *
         */
        void writeToIndex(void* data, VkDeviceSize index) { writeToBuffer(data, m_instanceSize, index * m_alignmentSize); }

        /**
         *  Flush the m_memory range at index * m_alignmentSize of the m_buffer to make it visible to the device
         *
         * @param index Used in offset calculation
         *
         */
        VkResult flushIndex(VkDeviceSize index) { return flush(m_alignmentSize, index * m_alignmentSize); }

        /**
         * Create a m_buffer info descriptor
         *
         * @param index Specifies the region given by index * m_alignmentSize
         *
         * @return VkDescriptorBufferInfo for instance at index
         */
        VkDescriptorBufferInfo descriptorInfoForIndex(VkDeviceSize index) { return descriptorInfo(m_alignmentSize, index * m_alignmentSize); }

        /**
         * Invalidate a m_memory range of the m_buffer to make it visible to the host
         *
         * @note Only required for non-coherent m_memory
         *
         * @param index Specifies the region to invalidate: index * m_alignmentSize
         *
         * @return VkResult of the invalidate call
         */
        VkResult invalidateIndex(VkDeviceSize index) { return invalidate(m_alignmentSize, index * m_alignmentSize); }

        VkBuffer getBuffer() const { return m_buffer; }
        void* getMappedMemory() const { return m_mapped; }
        uint32_t getInstanceCount() const { return m_instanceCount; }
        VkDeviceSize getInstanceSize() const { return m_instanceSize; }
        VkDeviceSize getAlignmentSize() const { return m_instanceSize; }
        VkBufferUsageFlags getUsageFlags() const { return m_usageFlags; }
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return m_memoryPropertyFlags; }
        VkDeviceSize getBufferSize() const { return m_bufferSize; }

    private:
        /**
         * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
         *
         * @param instanceSize The size of an instance
         * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
         * minUniformBufferOffsetAlignment)
         *
         * @return VkResult of the m_buffer mapping call
         */
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        Device& m_device;
        void* m_mapped = nullptr;
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;

        VkDeviceSize m_bufferSize;
        VkDeviceSize m_instanceSize;
        uint32_t m_instanceCount;
        VkDeviceSize m_alignmentSize;
        VkBufferUsageFlags m_usageFlags;
        VkMemoryPropertyFlags m_memoryPropertyFlags;

    }; // class Buffer

} // namespace ven
