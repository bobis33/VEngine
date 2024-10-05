///
/// @file Buffer.hpp
/// @brief This file contains the Buffer class
/// @namespace ven
///

#pragma once

#include "VEngine/Device.hpp"

namespace ven {

    ///
    /// @class Buffer
    /// @brief Class for buffer
    /// @namespace ven
    ///
    class Buffer {

        public:

            Buffer(Device& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
            ~Buffer();

            Buffer(const Buffer&) = delete;
            Buffer& operator=(const Buffer&) = delete;

            ///
            /// @brief Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
            ///
            /// @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete buffer range.
            /// @param offset (Optional) Byte offset from beginning
            ///
            /// @return VkResult of the buffer mapping call
            ///
            VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            ///
            /// @brief Unmap a mapped memory range
            ///
            /// @note Does not return a result as vkUnmapMemory can't fail
            ///
            void unmap();

            ///
            /// @brief Copies the specified data to the mapped buffer. Default value writes whole buffer range
            ///
            /// @param data Pointer to the data to copy
            /// @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer range.
            /// @param offset (Optional) Byte offset from beginning of mapped region
            ///
            void writeToBuffer(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

            ///
            /// @brief Flush a memory range of the buffer to make it visible to the device
            ///
            ///  @note Only required for non-coherent memory
            ///
            /// @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the complete buffer range.
            /// @param offset (Optional) Byte offset from beginning
            ///
            /// @return VkResult of the flush call
            ///
            [[nodiscard]] VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

            ///
            /// @brief Create a buffer info descriptor
            ///
            /// @param size (Optional) Size of the memory range of the descriptor
            /// @param offset (Optional) Byte offset from beginning
            ///
            /// @return VkDescriptorBufferInfo of specified offset and range
            ///
            [[nodiscard]] VkDescriptorBufferInfo descriptorInfo(const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0) const { return VkDescriptorBufferInfo{m_buffer, offset, size, }; }

            ///
            /// @brief Invalidate a memory range of the buffer to make it visible to the host
            ///
            /// @note Only required for non-coherent memory
            ///
            /// @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
            /// the complete buffer range.
            /// @param offset (Optional) Byte offset from beginning
            ///
            /// @return VkResult of the invalidate call
            ///
            [[nodiscard]] VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

            ///
            /// Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
            ///
            /// @param data Pointer to the data to copy
            /// @param index Used in offset calculation
            ///
            ///
            void writeToIndex(const void* data, const VkDeviceSize index) const { writeToBuffer(data, m_instanceSize, index * m_alignmentSize); }

            ///
            ///  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
            ///
            /// @param index Used in offset calculation
            ///
            [[nodiscard]] VkResult flushIndex(const VkDeviceSize index) const { return flush(m_alignmentSize, index * m_alignmentSize); }

            ///
            ///
            /// Create a buffer info descriptor
            ///
            /// @param index Specifies the region given by index * alignmentSize
            ///
            /// @return VkDescriptorBufferInfo for instance at index
            ///
            [[nodiscard]] VkDescriptorBufferInfo descriptorInfoForIndex(const VkDeviceSize index) const { return descriptorInfo(m_alignmentSize, index * m_alignmentSize); }

            ///
            /// Invalidate a memory range of the buffer to make it visible to the host
            ///
            /// @note Only required for non-coherent memory
            ///
            /// @param index Specifies the region to invalidate: index * alignmentSize
            ///
            /// @return VkResult of the invalidate call
            ///
            [[nodiscard]] VkResult invalidateIndex(const VkDeviceSize index) const { return invalidate(m_alignmentSize, index * m_alignmentSize); }

            [[nodiscard]] VkBuffer getBuffer() const { return m_buffer; }
            [[nodiscard]] void* getMappedMemory() const { return m_mapped; }
            [[nodiscard]] uint32_t getInstanceCount() const { return m_instanceCount; }
            [[nodiscard]] VkDeviceSize getInstanceSize() const { return m_instanceSize; }
            [[nodiscard]] VkDeviceSize getAlignmentSize() const { return m_instanceSize; }
            [[nodiscard]] VkBufferUsageFlags getUsageFlags() const { return m_usageFlags; }
            [[nodiscard]] VkMemoryPropertyFlags getMemoryPropertyFlags() const { return m_memoryPropertyFlags; }
            [[nodiscard]] VkDeviceSize getBufferSize() const { return m_bufferSize; }

        private:
            ///
            /// Returns the minimum instance size required to be compatible with devices minOffsetAlignment
            ///
            /// @param instanceSize The size of an instance
            /// @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
            /// minUniformBufferOffsetAlignment)
            ///
            /// @return VkResult of the buffer mapping call
            ///
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
