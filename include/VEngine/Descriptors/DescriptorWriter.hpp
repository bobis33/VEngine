///
/// @file DescriptorWriter.hpp
/// @brief This file contains the DescriptorsWriter class
/// @namespace ven
///

#pragma once

#include "VEngine/Descriptors/DescriptorPool.hpp"
#include "VEngine/Descriptors/DescriptorSetLayout.hpp"

namespace ven {

    ///
    /// @class DescriptorWriter
    /// @brief Class for descriptor writer
    /// @namespace ven
    ///
    class DescriptorWriter {

        public:

            DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool) :  m_setLayout{setLayout}, m_pool{pool} {}
            ~DescriptorWriter() = default;

            DescriptorWriter(const DescriptorWriter &) = delete;
            DescriptorWriter &operator=(const DescriptorWriter &) = delete;

            DescriptorWriter &writeBuffer(uint32_t binding, const VkDescriptorBufferInfo *bufferInfo);
            DescriptorWriter &writeImage(uint32_t binding, const VkDescriptorImageInfo *imageInfo);

            bool build(VkDescriptorSet &set);
            void overwrite(const VkDescriptorSet &set);

        private:

            DescriptorSetLayout &m_setLayout;
            DescriptorPool &m_pool;
            std::vector<VkWriteDescriptorSet> m_writes;

    }; // class DescriptorWriter

} // namespace ven
