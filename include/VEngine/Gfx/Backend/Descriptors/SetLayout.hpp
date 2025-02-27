///
/// @file SetLayout.hpp
/// @brief This file contains the DescriptorSetLayout class
/// @namespace ven
///

#pragma once

namespace ven {

    ///
    /// @class DescriptorSetLayout
    /// @brief Class for DescriptorSetLayout
    /// @namespace ven
    ///
    class DescriptorSetLayout {

        public:

            explicit DescriptorSetLayout(const VkDevice &device): m_device(device) { }
            ~DescriptorSetLayout() { vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr); }

            DescriptorSetLayout(const DescriptorSetLayout &) = delete;
            DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;
            DescriptorSetLayout(DescriptorSetLayout &&) = delete;
            DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

            void create(uint16_t textureSize);

            [[nodiscard]] const VkDescriptorSetLayout& getDescriptorSetLayout() const { return m_descriptorSetLayout; }

        private:

            const VkDevice& m_device;
            VkDescriptorSetLayout m_descriptorSetLayout = nullptr;

    }; // class DescriptorSetLayout

} // namespace ven
