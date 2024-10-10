///
/// @file FrameInfo.hpp
/// @brief This file contains the FrameInfo class
/// @namespace ven
///

#pragma once

#include <vulkan/vulkan.h>

#include "VEngine/Camera.hpp"
#include "VEngine/Object.hpp"
#include "VEngine/Light.hpp"

namespace ven {

static constexpr std::size_t MAX_LIGHTS = 10;

static constexpr float DEFAULT_AMBIENT_LIGHT_INTENSITY = .2F;
static constexpr glm::vec4 DEFAULT_AMBIENT_LIGHT_COLOR = {glm::vec3(1.F), DEFAULT_AMBIENT_LIGHT_INTENSITY};

    struct PointLightData
    {
        glm::vec4 position{};
        glm::vec4 color{};
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.F};
        glm::mat4 view{1.F};
        glm::mat4 inverseView{1.F};
        glm::vec4 ambientLightColor{DEFAULT_AMBIENT_LIGHT_COLOR};
        std::array<PointLightData, MAX_LIGHTS> pointLights;
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
        Light::Map &lights;
    };

} // namespace ven
