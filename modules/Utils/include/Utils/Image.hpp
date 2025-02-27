///
/// @file Image.hpp
/// @brief This file contains image struct
/// @namespace utl
///

#pragma once

#include <string>

namespace utl {

    struct Image {

        using pixel = unsigned char*;

        explicit Image(const std::string& path, bool flip = false);
        ~Image();

        pixel pixels = nullptr;
        int width = 0;
        int height = 0;
        int channels = 0;

    }; // struct Image

} // namespace utl
