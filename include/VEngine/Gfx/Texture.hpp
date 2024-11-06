///
/// @file Texture.hpp
/// @brief This file contains the Texture class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Core/Device.hpp"

namespace ven {

    ///
    /// @class Texture
    /// @brief Class for texture
    /// @namespace ven
    ///
    class Texture {

        public:

            Texture(Device &device, const std::string &textureFilepath);
            Texture(Device &device, VkFormat format, VkExtent3D extent, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount);
            ~Texture();

            Texture(const Texture &) = delete;
            Texture &operator=(const Texture &) = delete;

            void updateDescriptor();
            void transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout) const;

            [[nodiscard]] VkImageView imageView() const { return m_textureImageView; }
            [[nodiscard]] VkSampler sampler() const { return m_textureSampler; }
            [[nodiscard]] VkImage getImage() const { return m_textureImage; }
            [[nodiscard]] VkImageView getImageView() const { return m_textureImageView; }
            [[nodiscard]] VkDescriptorImageInfo getImageInfo() const { return m_descriptor; }
            [[nodiscard]] VkImageLayout getImageLayout() const { return m_textureLayout; }
            [[nodiscard]] VkExtent3D getExtent() const { return m_extent; }
            [[nodiscard]] VkFormat getFormat() const { return m_format; }

        private:

            void createTextureImage(const std::string &filepath);
            void createTextureImageView(VkImageViewType viewType);
            void createTextureSampler();

            VkDescriptorImageInfo m_descriptor{};
            Device &m_device;
            VkImage m_textureImage = nullptr;
            VkDeviceMemory m_textureImageMemory = nullptr;
            VkImageView m_textureImageView = nullptr;
            VkSampler m_textureSampler = nullptr;
            VkFormat m_format;
            VkImageLayout m_textureLayout{};
            uint32_t m_mipLevels{1};
            uint32_t m_layerCount{1};
            VkExtent3D m_extent{};

    }; // class Texture

}  // namespace ven
