/*
** @file Model.hpp
** @brief This file contains the Model class
** @namespace ven
*/

#pragma once

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VEngine/Device.hpp"

namespace ven {

    class Model {

        public:

            struct Vertex {
                glm::vec3 position;
                glm::vec3 color;

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            };

            struct Builder {
                std::vector<Vertex> vertices{};
                std::vector<uint32_t> indices{};
            };

            Model(Device &device, const Model::Builder &builder);
            ~Model();

            Model(const Model&) = delete;
            void operator=(const Model&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer) const;

        private:

            void createVertexBuffer(const std::vector<Vertex>& vertices);
            void createIndexBuffer(const std::vector<uint32_t>& indices);

            Device& m_device;
            VkBuffer m_vertexBuffer;
            VkDeviceMemory m_vertexBufferMemory;
            uint32_t m_vertexCount;

            bool m_hasIndexBuffer{false};
            VkBuffer m_indexBuffer;
            VkDeviceMemory m_indexBufferMemory;
            uint32_t m_indexCount;


    }; // class Model

} // namespace ven
