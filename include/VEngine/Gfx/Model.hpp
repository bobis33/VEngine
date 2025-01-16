///
/// @file Model.hpp
/// @brief This file contains the Model class
/// @namespace ven
///

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include "Texture.hpp"
#include "VEngine/Gfx/Buffer.hpp"

namespace ven {

    ///
    /// @class Model
    /// @brief Class for model
    /// @namespace ven
    ///
    class Model {

        public:

            using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

        struct Material {
            std::vector<std::shared_ptr<Texture>> diffuseTextures;
            std::vector<std::shared_ptr<Texture>> specularTextures;
            std::vector<std::shared_ptr<Texture>> normalTextures;
        };



            struct Vertex {
                glm::vec3 position{};
                glm::vec3 color{};
                glm::vec3 normal{};
                glm::vec2 uv{};

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

                bool operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
            };

        struct Mesh {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            Material material;
        };

            struct Builder {
                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
                TextureMap textures;
                std::unordered_map<const aiMesh*, std::shared_ptr<Texture>> m_meshTextures;
                std::vector<Mesh> meshes;

                void loadModel(Device& device, const std::string &filename);
                void processNode(Device& device, const aiNode* node, const aiScene* scene);
                void processMesh(Device& device, const aiMesh* mesh, const aiScene* scene);
            };

            Model(Device &device, const Builder &builder);
            ~Model() = default;

            Model(const Model&) = delete;
            Model& operator=(const Model&) = delete;
            Model(Model&&) = delete;
            Model& operator=(Model&&) = delete;

            void bind(VkCommandBuffer commandBuffer) const;
            void draw(VkCommandBuffer commandBuffer) const;
            void bindMesh(VkCommandBuffer commandBuffer, const Mesh& mesh) const;
            void drawMesh(VkCommandBuffer commandBuffer, const Mesh& mesh) const;

            TextureMap getTextures() const { return m_textures; }
            std::vector<Mesh> getMeshes() const { return m_meshes; }
        private:

            void createVertexBuffer(const std::vector<Vertex>& vertices);
            void createIndexBuffer(const std::vector<uint32_t>& indices);

            Device& m_device;
            std::unique_ptr<Buffer> m_vertexBuffer;
            uint32_t m_vertexCount;

            bool m_hasIndexBuffer{false};
            std::unique_ptr<Buffer> m_indexBuffer;
            uint32_t m_indexCount;
            TextureMap m_textures;
            std::vector<Mesh> m_meshes;
    }; // class Model

} // namespace ven
