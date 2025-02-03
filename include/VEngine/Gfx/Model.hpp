///
/// @file Model.hpp
/// @brief This file contains the Model class
/// @namespace ven
///

#pragma once

#include <unordered_map>

#include <assimp/scene.h>

#include "VEngine/Gfx/Buffer.hpp"
#include "VEngine/Gfx/Mesh.hpp"

namespace ven {

    static constexpr std::string_view MODEL_PATH = "assets/models/sponza/";

    ///
    /// @class Model
    /// @brief Class for model
    /// @namespace ven
    ///
    class Model {

        public:

            using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

            struct Builder {
                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
                TextureMap textures;
                std::vector<Mesh> meshes;

                void loadModel(const Device& device, const std::string &filename);
                void processNode(const Device& device, const aiNode* node, const aiScene* scene);
                void processMesh(const aiMesh* mesh);
                void processMaterial(const Device &device, const aiMesh *mesh, const aiScene *scene);
            };

            Model(const Device &device, const Builder &builder);
            ~Model() = default;

            Model(const Model&) = delete;
            Model& operator=(const Model&) = delete;
            Model(Model&&) = delete;
            Model& operator=(Model&&) = delete;

            void bind(VkCommandBuffer commandBuffer) const;
            void draw(VkCommandBuffer commandBuffer) const;
            void bindMesh(VkCommandBuffer commandBuffer, const Mesh& mesh) const;
            void drawMesh(VkCommandBuffer commandBuffer, const Mesh& mesh) const;

            const TextureMap& getTextures() const { return m_textures; }
            const std::vector<Mesh>& getMeshes() const { return m_meshes; }

        private:

            void createVertexBuffer(const std::vector<Vertex>& vertices);
            void createIndexBuffer(const std::vector<uint32_t>& indices);

            const Device& m_device;
            std::unique_ptr<Buffer> m_vertexBuffer;
            uint32_t m_vertexCount;

            bool m_hasIndexBuffer{false};
            std::unique_ptr<Buffer> m_indexBuffer;
            uint32_t m_indexCount;
            TextureMap m_textures;
            std::vector<Mesh> m_meshes;
    }; // class Model

} // namespace ven
