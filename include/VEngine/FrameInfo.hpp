/*
** @file FrameInfo.hpp
** @brief This file contains the FrameInfo class
** @namespace ven
*/

#pragma once

#include <vulkan/vulkan.h>

#include "VEngine/Camera.hpp"

namespace ven {

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
    };

} // namespace ven
