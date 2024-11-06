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

#include "VEngine/Gfx/Buffer.hpp"
#include "VEngine/Utils/Logger.hpp"


namespace ven {

    ///
    /// @class Model
    /// @brief Class for model
    /// @namespace ven
    ///
    class Model {

        public:

            struct Vertex {
                glm::vec3 position{};
                glm::vec3 color{};
                glm::vec3 normal{};
                glm::vec2 uv{};

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

                bool operator==(const Vertex& other) const {
                    return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
                }
            };

            struct Builder {
                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;

                void loadModel(const std::string &filename);
                void processNode(const aiNode* node, const aiScene* scene);
                void processMesh(const aiMesh* mesh, const aiScene* scene);
            };

            Model(Device &device, const Builder &builder);
            ~Model() = default;

            Model(const Model&) = delete;
            void operator=(const Model&) = delete;

            static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &filename);

            void bind(VkCommandBuffer commandBuffer) const;
            void draw(VkCommandBuffer commandBuffer) const;

        static std::unordered_map<std::string, std::shared_ptr<Model>> loadAllModels(const std::string &directoryPath, Device &device) {
            std::unordered_map<std::string, std::shared_ptr<Model>> modelCache;

            for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    Logger::logExecutionTime("Creating model " + entry.path().string(), [&]() {
                        const std::string &filepath = entry.path().string();
                        modelCache[filepath] = Model::createModelFromFile(device, filepath);
                    });

                    const std::string &filepath = entry.path().string();
                    modelCache[filepath] = Model::createModelFromFile(device, filepath);
                }
            }

            return modelCache;
        }


        private:

            void createVertexBuffer(const std::vector<Vertex>& vertices);
            void createIndexBuffer(const std::vector<uint32_t>& indices);

            Device& m_device;
            std::unique_ptr<Buffer> m_vertexBuffer;
            uint32_t m_vertexCount;

            bool m_hasIndexBuffer{false};
            std::unique_ptr<Buffer> m_indexBuffer;
            uint32_t m_indexCount;

    }; // class Model

} // namespace ven
