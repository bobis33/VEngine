///
/// @file Utils.hpp
/// @brief This file contains utils for VEngine
/// @namespace ven
///

#pragma once

#include <cstdint>

namespace ven {

    enum ENGINE_STATE : uint8_t {
        EDITOR = 0,
        PLAYER = 1,
        PAUSED = 2,
        EXIT = 3
    };

} // namespace ven
