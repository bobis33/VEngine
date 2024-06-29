/*
** @file FrameInfo.hpp
** @brief This file contains the FrameInfo class
** @namespace ven
*/

#pragma once

#include <vulkan/vulkan.h>

#include "VEngine/Camera.hpp"
#include "VEngine/Object.hpp"

namespace ven {

static constexpr const std::size_t MAX_LIGHTS = 10;

    struct PointLight
    {
        glm::vec4 position{};
        glm::vec4 color{};
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.F};
        glm::mat4 view{1.F};
        glm::vec4 ambientLightColor{1.F, 1.F, 1.F, .02F};
        std::array<PointLight, MAX_LIGHTS> pointLights;
        int numLights;
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
        VkDescriptorSet globalDescriptorSet;
        Object::Map &objects;
    };

} // namespace ven
