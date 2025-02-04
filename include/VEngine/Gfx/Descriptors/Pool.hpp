///
/// @file Pool.hpp
/// @brief This file contains the DescriptorPool class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Core/Device.hpp"

namespace ven {

    static constexpr uint32_t DEFAULT_MAX_SETS = 1000;

    ///
    /// @class DescriptorPool
    /// @brief Class for descriptor pool
    /// @namespace ven
    ///
    class DescriptorPool {

        public:

            class Builder {

                public:

                    explicit Builder(const Device &device) : m_device{device} {}

                    [[nodiscard]] std::unique_ptr<DescriptorPool> build() const { return std::make_unique<DescriptorPool>(m_device, m_maxSets, m_poolFlags, m_poolSizes); }

                    Builder &addPoolSize(const VkDescriptorType descriptorType, const uint32_t count) { m_poolSizes.push_back({descriptorType, count}); return *this; }
                    Builder &setPoolFlags(const VkDescriptorPoolCreateFlags flags) { m_poolFlags = flags; return *this; }
                    Builder &setMaxSets(const uint32_t count) { m_maxSets = count; return *this; }

                private:

                    const Device &m_device;
                    std::vector<VkDescriptorPoolSize> m_poolSizes;
                    uint32_t m_maxSets{DEFAULT_MAX_SETS};
                    VkDescriptorPoolCreateFlags m_poolFlags{0};

            }; // class Builder

            DescriptorPool(const Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes);
            ~DescriptorPool() { vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr); }

            DescriptorPool(const DescriptorPool &) = delete;
            DescriptorPool &operator=(const DescriptorPool &) = delete;
            DescriptorPool(DescriptorPool &&) = delete;
            DescriptorPool &operator=(DescriptorPool &&) = delete;

            bool allocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
            void freeDescriptors(const std::vector<VkDescriptorSet> &descriptors) const { vkFreeDescriptorSets(m_device.device(), m_descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data()); }
            void resetPool() const { vkResetDescriptorPool(m_device.device(), m_descriptorPool, 0); }

            [[nodiscard]] VkDescriptorPool getDescriptorPool() const { return m_descriptorPool; }

        private:

            const Device &m_device;
            VkDescriptorPool m_descriptorPool;
            friend class DescriptorWriter;

    }; // class DescriptorPool

} // namespace ven
