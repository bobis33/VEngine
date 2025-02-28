///
/// @file Texture.hpp
/// @brief This file contains the Texture class
/// @namespace ven
///

#pragma once

#include "VEngine/Gfx/Backend/SwapChain.hpp"

namespace ven {

    ///
    /// @class Texture
    /// @brief Class for texture
    /// @namespace ven
    ///
    class Texture {

        public:

            explicit Texture(const Device& device, const SwapChain& swapChain, const std::string& path) : m_device{device}, m_swapChain{swapChain} { createTextureSampler(); createTextureImage(path, swapChain); swapChain.createImageView(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, m_textureImageView); }
            ~Texture() = default;

            Texture(const Texture &) = delete;
            Texture &operator=(const Texture &) = delete;
            Texture(Texture &&) = delete;
            Texture &operator=(Texture &&) = delete;

            void createTextureSampler();
            void createTextureImage(const std::string& filePath, const SwapChain& swapChain);
            void createDefaultTextureImage(const SwapChain& swapChain);
            void clean() const { const VkDevice& device = m_device.getVkDevice(); vkDestroySampler(device, m_textureSampler, nullptr); vkDestroyImageView(device, m_textureImageView, nullptr); vkDestroyImage(device, m_textureImage, nullptr); vkFreeMemory(device, m_textureImageMemory, nullptr); }

            [[nodiscard]] const VkSampler& getTextureSampler() const { return m_textureSampler; }
            [[nodiscard]] uint32_t getMipLevels() const { return m_mipLevels; }
            [[nodiscard]] const VkImage& getTextureImage() const { return m_textureImage; }
            [[nodiscard]] const VkDeviceMemory& getTextureImageMemory() const { return m_textureImageMemory; }
            [[nodiscard]] const VkImageView& getTextureImageView() const { return m_textureImageView; }

        private:

            void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) const;

            const Device& m_device;
            const SwapChain& m_swapChain;
            VkSampler m_textureSampler = VK_NULL_HANDLE;
            uint32_t m_mipLevels = 0;
            VkImage m_textureImage = VK_NULL_HANDLE;
            VkDeviceMemory m_textureImageMemory = VK_NULL_HANDLE;
            VkImageView m_textureImageView = VK_NULL_HANDLE;

    }; // class Texture

} // namespace ven
