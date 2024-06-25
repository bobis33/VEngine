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

            Model(Device &device, const std::vector<Vertex> &vertices);
            ~Model();

            Model(const Model&) = delete;
            void operator=(const Model&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);

        private:

            void createVertexBuffer(const std::vector<Vertex>& vertices);

            Device& m_device;
            VkBuffer m_vertexBuffer;
            VkDeviceMemory m_vertexBufferMemory;
            uint32_t m_vertexCount;

    }; // class Model

} // namespace ven
