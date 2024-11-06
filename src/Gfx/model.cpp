#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "VEngine/Gfx/Model.hpp"
#include "VEngine/Utils/Colors.hpp"
#include "VEngine/Utils/HashCombine.hpp"

template<>
struct std::hash<ven::Model::Vertex> {
    size_t operator()(ven::Model::Vertex const &vertex) const noexcept {
        size_t seed = 0;
        ven::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    }
};

ven::Model::Model(Device &device, const Builder &builder) : m_device{device}, m_vertexCount(0), m_indexCount(0)
{
    createVertexBuffer(builder.vertices);
    createIndexBuffer(builder.indices);
}

void ven::Model::createVertexBuffer(const std::vector<Vertex> &vertices)
{
    m_vertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
    constexpr unsigned long vertexSize = sizeof(vertices[0]);
    const VkDeviceSize bufferSize = vertexSize * m_vertexCount;

    Buffer stagingBuffer{m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.map();
    stagingBuffer.writeToBuffer(vertices.data());

    m_vertexBuffer = std::make_unique<Buffer>(m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_device.copyBuffer(stagingBuffer.getBuffer(), m_vertexBuffer->getBuffer(), bufferSize);
}

void ven::Model::createIndexBuffer(const std::vector<uint32_t> &indices)
{
    m_indexCount = static_cast<uint32_t>(indices.size());
    m_hasIndexBuffer = m_indexCount > 0;

    if (!m_hasIndexBuffer) {
        return;
    }

    constexpr uint32_t indexSize = sizeof(indices[0]);

    Buffer stagingBuffer{m_device, indexSize, m_indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.map();
    stagingBuffer.writeToBuffer(indices.data());

    m_indexBuffer = std::make_unique<Buffer>(m_device, indexSize, m_indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_device.copyBuffer(stagingBuffer.getBuffer(), m_indexBuffer->getBuffer(), sizeof(indices[0]) * m_indexCount);
}

void ven::Model::draw(const VkCommandBuffer commandBuffer) const
{
    if (m_hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
    }
}

void ven::Model::bind(const VkCommandBuffer commandBuffer) const
{
    const std::array buffers{m_vertexBuffer->getBuffer()};
    constexpr std::array<VkDeviceSize, 1> offsets{0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());

    if (m_hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

std::vector<VkVertexInputBindingDescription> ven::Model::Vertex::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> ven::Model::Vertex::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
    attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
    attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
    attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

    return attributeDescriptions;
}

void ven::Model::Builder::loadModel(const std::string &filename)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U) || (scene->mRootNode == nullptr)) {
        throw std::runtime_error("Failed to load model with Assimp: " + std::string(importer.GetErrorString()));
    }

    vertices.clear();
    indices.clear();

    processNode(scene->mRootNode, scene);
}

void ven::Model::Builder::processNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void ven::Model::Builder::processMesh(const aiMesh* mesh, const aiScene* scene) {
    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }

        if (mesh->mTextureCoords[0] != nullptr) {
            vertex.uv = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.uv = glm::vec2(0.0F, 0.0F);
        }

        if (!uniqueVertices.contains(vertex)) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
        }

        indices.push_back(uniqueVertices[vertex]);
    }
}

