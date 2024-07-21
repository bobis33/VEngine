///
/// @file Constant.hpp
/// @brief This file contains the constant values used in the project
/// @namespace ven
///

#pragma once

namespace ven {

    static constexpr uint32_t DEFAULT_WIDTH = 1920;
    static constexpr uint32_t DEFAULT_HEIGHT = 1080;

    static constexpr std::string_view DEFAULT_TITLE = "VEngine";
    static constexpr std::string_view SHADERS_BIN_PATH = "shaders/bin/";

    using return_type_t = enum ReturnType : uint8_t {
        VEN_SUCCESS = 0,
        VEN_FAILURE = 1
    };

} // namespace ven
