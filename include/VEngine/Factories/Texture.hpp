///
/// @file Texture.hpp
/// @brief This file contains the Texture Factory class
/// @namespace ven
///

#pragma once

#include <memory>
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

            static std::unique_ptr<Texture> create(Device& device, const std::string& filepath) { return std::make_unique<Texture>(device, filepath); }
            static std::unordered_map<std::string, std::shared_ptr<Texture>> loadAll(Device& device, const std::string& folderPath);

    }; // class TextureFactory

} // namespace ven
