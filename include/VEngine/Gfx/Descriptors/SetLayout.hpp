///
/// @file SetLayout.hpp
/// @brief This file contains the DescriptorSetLayout class
/// @namespace ven
///

#pragma once

#include <memory>
#include <unordered_map>

#include "VEngine/Core/Device.hpp"

namespace ven {

    ///
    /// @class DescriptorSetLayout
    /// @brief Class for descriptor set layout
    /// @namespace ven
    ///
    class DescriptorSetLayout {

        public:

            class Builder {

                public:

                    explicit Builder(const Device &device) : m_device{device} {}

                    Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
                    std::unique_ptr<DescriptorSetLayout> build() const { return std::make_unique<DescriptorSetLayout>(m_device, m_bindings); }

                private:

                    const Device &m_device;
                    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

            }; // class Builder

            DescriptorSetLayout(const Device &device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings);
            ~DescriptorSetLayout() { vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr); }

            DescriptorSetLayout(const DescriptorSetLayout &) = delete;
            DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;
            DescriptorSetLayout(DescriptorSetLayout &&) = delete;
            DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

            VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

        private:

            const Device &m_device;
            VkDescriptorSetLayout m_descriptorSetLayout;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

            friend class DescriptorWriter;

    }; // class DescriptorSetLayout

} // namespace ven
