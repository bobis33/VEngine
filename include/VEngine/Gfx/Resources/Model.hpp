///
/// @file Model.hpp
/// @brief This file contains the model class
/// @namespace ven
///

#pragma once

#include <assimp/scene.h>

#include "VEngine/Gfx/Resources/Mesh.hpp"
#include "VEngine/Gfx/Resources/Transform.hpp"

namespace ven {

    ///
    /// @class Model
    /// @brief Class for models
    /// @namespace ven
    ///
    class Model {

        public:

            using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

            explicit Model(const Device& device, const SwapChain& swapChain, const std::string& path);
            ~Model() = default;

            Model(const Model&) = delete;
            Model& operator=(const Model&) = delete;
            Model(Model&&) noexcept = default;
            Model& operator=(Model&&) noexcept = delete;

            template<typename T>
            static void createBuffer(const Device& device, const std::vector<T>& data, const VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
                void* mappedData = nullptr;
                VkBuffer stagingBuffer = nullptr;
                VkDeviceMemory stagingBufferMemory = nullptr;
                const VkDeviceSize bufferSize = sizeof(data[0]) * data.size();
                const VkDevice& vkDevice = device.getVkDevice();
                device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
                vkMapMemory(vkDevice, stagingBufferMemory, 0, bufferSize, 0, &mappedData);
                memcpy(mappedData, data.data(), static_cast<size_t>(bufferSize));
                vkUnmapMemory(vkDevice, stagingBufferMemory);
                device.createBuffer(bufferSize, usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
                device.copyBuffer(stagingBuffer, buffer, bufferSize);
                vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
                vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);
            }

            [[nodiscard]] const std::vector<std::unique_ptr<Mesh>>& getMeshes() const { return m_meshes; }
            [[nodiscard]] Transform& getTransform() { return m_transform; }

        private:

            const Device& m_device;
            const SwapChain& m_swapChain;
            std::vector<std::unique_ptr<Mesh>> m_meshes;
            Transform m_transform;

            void processNode(const aiNode* node, const aiScene* scene, const Device& device, const SwapChain& swapChain, const glm::mat4 &parentTransform);
            static std::unique_ptr<Mesh> processMesh(const aiMesh* mesh, const aiScene* scene, const Device& device, const SwapChain& swapChain, const glm::mat4& transform);

    }; // class Model

} // namespace ven
