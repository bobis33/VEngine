///
/// @file Pool.hpp
/// @brief This file contains the DescriptorPool class
/// @namespace ven
///

#pragma once

namespace ven {

    ///
    /// @class DescriptorPool
    /// @brief Class for DescriptorPool
    /// @namespace ven
    ///
    class DescriptorPool {

        public:

            explicit DescriptorPool(const VkDevice& device);
            ~DescriptorPool() { vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr); }

            DescriptorPool(const DescriptorPool &) = delete;
            DescriptorPool &operator=(const DescriptorPool &) = delete;
            DescriptorPool(DescriptorPool &&) = delete;
            DescriptorPool &operator=(DescriptorPool &&) = delete;

            [[nodiscard]] const VkDescriptorPool& getDescriptorPool() const { return m_descriptorPool; }

        private:

            const VkDevice& m_device;
            VkDescriptorPool m_descriptorPool = nullptr;

    }; // class DescriptorPool

} // namespace ven
