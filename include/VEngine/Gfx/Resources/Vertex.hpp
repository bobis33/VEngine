///
/// @file Vertex.hpp
/// @brief This file contains the Vertex struct
/// @namespace ven
///

#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace ven {

    struct Vertex {

        glm::vec3 pos{};
        glm::vec3 color{};
        glm::vec2 texCoord{};
        uint32_t textureIndex;

        bool operator==(const Vertex& other) const { return pos == other.pos && color == other.color && texCoord == other.texCoord && textureIndex == other.textureIndex; }

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() { return {{.binding=0, .stride=sizeof(Vertex), .inputRate=VK_VERTEX_INPUT_RATE_VERTEX}}; }
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() { return {
            {.location=0, .binding=0, .format=VK_FORMAT_R32G32B32_SFLOAT, .offset=offsetof(Vertex, pos)},
            {.location=1, .binding=0, .format=VK_FORMAT_R32G32B32_SFLOAT, .offset=offsetof(Vertex, color)},
            {.location=2, .binding=0, .format=VK_FORMAT_R32G32_SFLOAT, .offset=offsetof(Vertex, texCoord)},
            {.location=3, .binding=0, .format=VK_FORMAT_R32_UINT, .offset=offsetof(Vertex, textureIndex)}
        }; }

    };

} // namespace ven
