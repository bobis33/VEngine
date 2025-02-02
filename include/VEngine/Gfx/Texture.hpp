///
/// @file Texture.hpp
/// @brief This file contains the Texture class
/// @namespace ven
///

#pragma once

#include "VEngine/Core/Device.hpp"

namespace ven {

    ///
    /// @class Texture
    /// @brief Class for texture
    /// @namespace ven
    ///
    class Texture {

        public:

            Texture(const Device &device, const std::string &textureFilepath);
            Texture(const Device &device, VkFormat format, VkExtent3D extent, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount);
            ~Texture();

            Texture(const Texture &) = delete;
            Texture &operator=(const Texture &) = delete;
            Texture(Texture &&) = delete;
            Texture &operator=(Texture &&) = delete;

            void updateDescriptor();
            void transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout) const;

            [[nodiscard]] const VkImageView& imageView() const { return m_textureImageView; }
            [[nodiscard]] const VkSampler& sampler() const { return m_textureSampler; }
            [[nodiscard]] const VkImage& getImage() const { return m_textureImage; }
            [[nodiscard]] const VkImageView& getImageView() const { return m_textureImageView; }
            [[nodiscard]] const VkDescriptorImageInfo& getImageInfo() const { return m_descriptor; }
            [[nodiscard]] const VkImageLayout& getImageLayout() const { return m_textureLayout; }
            [[nodiscard]] const VkExtent3D& getExtent() const { return m_extent; }
            [[nodiscard]] const VkFormat& getFormat() const { return m_format; }

        private:

            void createTextureImage(const std::string &filepath);
            void createTextureImageView(VkImageViewType viewType);
            void createTextureSampler();

            const Device &m_device;
            VkDescriptorImageInfo m_descriptor{};
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
