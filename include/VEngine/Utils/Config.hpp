///
/// @file Config.hpp
/// @brief
/// @namespace ven
///

#pragma once

#ifdef _WIN32
    #undef near
    #undef far
#endif

#include "VEngine/Core/Window.hpp"
#include "VEngine/Scene/Camera.hpp"

namespace ven {

    struct WindowConf
    {
        uint16_t width = DEFAULT_WIDTH;
        uint16_t height = DEFAULT_HEIGHT;
        bool fullscreen = false; // TODO: Implement fullscreen
    };

    struct CameraConf
    {
        float fov = DEFAULT_FOV;
        float move_speed = DEFAULT_MOVE_SPEED;
        float look_speed = DEFAULT_LOOK_SPEED;
        float near = DEFAULT_NEAR;
        float far = DEFAULT_FAR;
    };

    struct Config
    {
        WindowConf window;
        CameraConf camera;
        bool vsync = false; // TODO: Implement vsync
    };

} // namespace ven
