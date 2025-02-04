///
/// @file FrameInfo.hpp
/// @brief This file contains the FrameInfo class
/// @namespace ven
///

#pragma once

#include "VEngine/Gfx/Descriptors/Pool.hpp"
#include "VEngine/Scene/Entities/Object.hpp"
#include "VEngine/Scene/Entities/Light.hpp"

namespace ven {

static constexpr float DEFAULT_AMBIENT_LIGHT_INTENSITY = .2F;
static constexpr glm::vec4 DEFAULT_AMBIENT_LIGHT_COLOR = {glm::vec3(1.F), DEFAULT_AMBIENT_LIGHT_INTENSITY};

    struct PointLightData
    {
        glm::vec4 position{};
        glm::vec4 color{};
        float shininess{32.F};
        float padding[3]; // Pad to 32 bytes
    };

    struct ObjectBufferData {
        glm::mat4 modelMatrix{1.F};
        glm::mat4 normalMatrix{1.F};
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.F};
        glm::mat4 view{1.F};
        glm::mat4 inverseView{1.F};
        glm::vec4 ambientLightColor{DEFAULT_AMBIENT_LIGHT_COLOR};
        std::array<PointLightData, MAX_LIGHTS> pointLights;
        uint8_t numLights;
    };

    struct FrameInfo
    {
        unsigned long frameIndex;
        VkCommandBuffer commandBuffer;
        VkDescriptorSet globalDescriptorSet;
        DescriptorPool &frameDescriptorPool;
        Object::Map &objects;
        Light::Map &lights;
    };

} // namespace ven
