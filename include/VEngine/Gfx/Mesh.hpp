///
/// @file Mesh.hpp
/// @brief This file contains the Mesh class
/// @namespace ven
///

#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "VEngine/Gfx/Texture.hpp"

namespace ven {

    ///
    /// @class Mesh
    /// @brief Class for mesh
    /// @namespace ven
    ///

    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
    };

    struct Material {
        std::vector<std::shared_ptr<Texture>> diffuseTextures;
        std::vector<std::shared_ptr<Texture>> specularTextures;
        std::vector<std::shared_ptr<Texture>> normalTextures;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        Material material;
    };

} // namespace ven
