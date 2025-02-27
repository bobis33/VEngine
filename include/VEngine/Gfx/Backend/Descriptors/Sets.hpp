///
/// @file Pool.hpp
/// @brief This file contains the DescriptorSets class
/// @namespace ven
///

#pragma once

namespace ven {

    ///
    /// @class DescriptorSets
    /// @brief Class for DescriptorSets
    /// @namespace ven
    ///
    class DescriptorSets {

        public:

            explicit DescriptorSets(const VkDevice& device, const VkDescriptorPool& descriptorPool, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<VkBuffer>& buffers) : m_device(device), m_descriptorPool(descriptorPool), m_descriptorSetLayout(descriptorSetLayout), m_buffers(buffers) { }
            ~DescriptorSets() = default;

            DescriptorSets(const DescriptorSets &) = delete;
            DescriptorSets &operator=(const DescriptorSets &) = delete;
            DescriptorSets(DescriptorSets &&) = delete;
            DescriptorSets &operator=(DescriptorSets &&) = delete;

            void create(VkDeviceSize bufferSize);

            [[nodiscard]] const std::vector<VkDescriptorSet>& getDescriptorSets() const { return m_descriptorSets; }

        private:

            const VkDevice& m_device;
            const VkDescriptorPool& m_descriptorPool;
            const VkDescriptorSetLayout& m_descriptorSetLayout;
            const std::vector<VkBuffer>& m_buffers;
            std::vector<VkDescriptorSet> m_descriptorSets;

    }; // class DescriptorSets

} // namespace ven
