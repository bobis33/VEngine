///
/// @file DescriptorPool.hpp
/// @brief This file contains the DescriptorPool class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Device.hpp"

namespace ven {

    ///
    /// @class DescriptorPool
    /// @brief Class for descriptor pool
    /// @namespace ven
    ///
    class DescriptorPool {

        public:

            class Builder {

                public:

                    explicit Builder(Device &device) : m_device{device} {}

                    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
                    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
                    Builder &setMaxSets(uint32_t count);
                    [[nodiscard]] std::unique_ptr<DescriptorPool> build() const { return std::make_unique<DescriptorPool>(m_device, m_maxSets, m_poolFlags, m_poolSizes); }

                private:

                    Device &m_device;
                    std::vector<VkDescriptorPoolSize> m_poolSizes;
                    uint32_t m_maxSets = 1000;
                    VkDescriptorPoolCreateFlags m_poolFlags = 0;

            }; // class Builder

            DescriptorPool(Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes);
            ~DescriptorPool() { vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr); }
            DescriptorPool(const DescriptorPool &) = delete;
            DescriptorPool &operator=(const DescriptorPool &) = delete;

            bool allocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
            void freeDescriptors(const std::vector<VkDescriptorSet> &descriptors) const { vkFreeDescriptorSets(m_device.device(), m_descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data()); }
            void resetPool() const { vkResetDescriptorPool(m_device.device(), m_descriptorPool, 0); }

            [[nodiscard]] VkDescriptorPool getDescriptorPool() const { return m_descriptorPool; }

        private:

            Device &m_device;
            VkDescriptorPool m_descriptorPool;
            friend class DescriptorWriter;

    }; // class DescriptorPool

} // namespace ven
