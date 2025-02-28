///
/// @file Mesh.hpp
/// @brief This file contains the mesh class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Gfx/Resources/Vertex.hpp"
#include "VEngine/Gfx/Resources/Texture.hpp"

namespace ven {

    class Mesh {

        public:

            Mesh() = default;
            ~Mesh() = default;

            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;
            Mesh(Mesh&&) = delete;
            Mesh& operator=(Mesh&&) = delete;

            void addVertex(const Vertex& vertex) { m_vertices.push_back(vertex); }
            void addIndices(const uint32_t indices) { m_indices.push_back(indices); }
            void setTextureIndex(const uint32_t index) { for (auto& vertex : m_vertices) { vertex.textureIndex = index; } }

            [[nodiscard]] const std::vector<Vertex>& getVertices() const { return m_vertices; }
            [[nodiscard]] const std::vector<uint32_t>& getIndices() const { return m_indices; }

        private:

            std::vector<Vertex> m_vertices;
            std::vector<uint32_t> m_indices;

    };

} // namespace ven
