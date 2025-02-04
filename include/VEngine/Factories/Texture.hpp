///
/// @file Texture.hpp
/// @brief This file contains the Texture Factory class
/// @namespace ven
///

#pragma once

#include <unordered_map>

#include "VEngine/Gfx/Texture.hpp"

namespace ven {

    ///
    /// @class TextureFactory
    /// @brief Class for Texture factory
    /// @namespace ven
    ///
    class TextureFactory {

        public:

            TextureFactory() = delete;
            ~TextureFactory() = default;

            TextureFactory(const TextureFactory&) = delete;
            TextureFactory& operator=(const TextureFactory&) = delete;
            TextureFactory(TextureFactory&&) = delete;
            TextureFactory& operator=(TextureFactory&&) = delete;

            static std::unique_ptr<Texture> create(const Device& device, const std::string& filepath) { return std::make_unique<Texture>(device, filepath); }
            static std::unordered_map<std::string, std::shared_ptr<Texture>> loadAll(Device& device, const std::string& folderPath);

    }; // class TextureFactory

} // namespace ven
