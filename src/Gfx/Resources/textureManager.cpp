#include <filesystem>

#include "VEngine/Gfx/Resources/TextureManager.hpp"

void ven::TextureManager::loadTextures(const Device& device, const SwapChain& swapChain, const std::string& directory) {
    auto& instance = getInstance();
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto& path = entry.path();
        const std::string& filename = path.string();
        if (const auto& extension = path.extension();
            instance.m_texturePaths.contains(filename) || extension != ".png" && extension != ".jpg") {
            continue;
        }
        const uint8_t newIndex = instance.m_nextIndex++;
        const auto texture = std::make_shared<Texture>(device, swapChain, filename);
        instance.m_textures[newIndex] = texture;
        instance.m_texturePaths[filename] = newIndex;
    }
}

void ven::TextureManager::loadTexture(const Device& device, const SwapChain& swapChain, const std::string& filepath) {
    auto& instance = getInstance();
    if (instance.m_texturePaths.contains(filepath)) {
        return;
    }
    const uint8_t newIndex = instance.m_nextIndex++;
    const auto texture = std::make_shared<Texture>(device, swapChain, filepath);
    instance.m_textures[newIndex] = texture;
    instance.m_texturePaths[filepath] = newIndex;
}


void ven::TextureManager::clean() {
    for (const auto& [key, texture] : getInstance().m_textures) {
        texture.get()->clean();
    }
}
