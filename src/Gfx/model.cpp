#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "VEngine/Gfx/Mesh.hpp"
#include "VEngine/Gfx/Model.hpp"
#include "VEngine/Utils/Colors.hpp"
#include "VEngine/Utils/HashCombine.hpp"

template<>
struct std::hash<ven::Vertex> {
    size_t operator()(ven::Vertex const &vertex) const noexcept {
        size_t seed = 0;
        ven::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    }
};

ven::Model::Model(Device &device, const Builder &builder) : m_device{device}, m_vertexCount(0), m_indexCount(0), m_textures(builder.textures), m_meshes(builder.meshes)
{
    createVertexBuffer(builder.vertices);
    createIndexBuffer(builder.indices);
}

void ven::Model::createVertexBuffer(const std::vector<Vertex> &vertices)
{
    m_vertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
    constexpr unsigned long vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer{m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.map();
    stagingBuffer.writeToBuffer(vertices.data());

    m_vertexBuffer = std::make_unique<Buffer>(m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_device.copyBuffer(stagingBuffer.getBuffer(), m_vertexBuffer->getBuffer(), vertexSize * m_vertexCount);
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

    m_device.copyBuffer(stagingBuffer.getBuffer(), m_indexBuffer->getBuffer(), indexSize * m_indexCount);
}

void ven::Model::draw(const VkCommandBuffer commandBuffer) const
{
    if (m_hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
    }
}

void ven::Model::drawMesh(const VkCommandBuffer commandBuffer, const Mesh& mesh) const {
    if (m_hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.indices.size()), 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, static_cast<uint32_t>(mesh.vertices.size()), 1, 0, 0);
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

void ven::Model::bindMesh(const VkCommandBuffer commandBuffer, const Mesh& mesh) const
{
    const std::array buffers{m_vertexBuffer->getBuffer()};
    constexpr std::array<VkDeviceSize, 1> offsets{0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());

    if (m_hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

void ven::Model::Builder::loadModel(Device& device, const std::string &filename)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

    if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U) || (scene->mRootNode == nullptr)) {
        throw std::runtime_error("Failed to load model with Assimp: " + std::string(importer.GetErrorString()));
    }

    vertices.clear();
    indices.clear();

    processNode(device, scene->mRootNode, scene);
}

void ven::Model::Builder::processNode(Device& device, const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
        processMaterial(device, mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(device, node->mChildren[i], scene);
    }
}

void loadMaterialTextures(ven::Device& device, const aiMaterial* material, const aiTextureType type, aiString &texturePath, std::unordered_map<std::string, std::shared_ptr<ven::Texture>>& textures, std::vector<std::shared_ptr<ven::Texture>>& meshTextures) {

    if (material->GetTexture(type, 0, &texturePath) == AI_SUCCESS) {
        const std::string fullPath = std::filesystem::absolute(ven::MODEL_PATH.data() + std::string(texturePath.C_Str())).string();

        if (!textures.contains(fullPath)) {
            std::cout << "Loading texture: " << fullPath << '\n';
            textures[fullPath] = std::make_shared<ven::Texture>(device, fullPath);
        }
        meshTextures.push_back(textures[fullPath]);
    }
}

void ven::Model::Builder::processMaterial(Device &device, const aiMesh *mesh, const aiScene *scene)
{
    Material meshMaterial{};

    if (mesh->mMaterialIndex >= 0) {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texturePath;

        loadMaterialTextures(device, material, aiTextureType_DIFFUSE, texturePath, textures, meshMaterial.diffuseTextures);
        loadMaterialTextures(device, material, aiTextureType_SPECULAR, texturePath, textures, meshMaterial.specularTextures);
        // loadMaterialTextures(device, material, aiTextureType_AMBIENT, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_EMISSIVE, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_HEIGHT, texturePath, textures, meshMaterial);
        loadMaterialTextures(device, material, aiTextureType_NORMALS, texturePath, textures, meshMaterial.normalTextures);
        // loadMaterialTextures(device, material, aiTextureType_SHININESS, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_OPACITY, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_DISPLACEMENT, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_LIGHTMAP, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_REFLECTION, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_BASE_COLOR, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_NORMAL_CAMERA, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_EMISSION_COLOR, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_METALNESS, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_DIFFUSE_ROUGHNESS, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_AMBIENT_OCCLUSION, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_UNKNOWN, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_SHEEN, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_CLEARCOAT, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_TRANSMISSION, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_MAYA_BASE, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_MAYA_SPECULAR, texturePath, textures, meshMaterial);
        // loadMaterialTextures(device, material, aiTextureType_MAYA_SPECULAR_COLOR, texturePath, textures, meshMaterial);
    }

    meshes.push_back({ .vertices = vertices, .indices = indices, .material = meshMaterial });

    /**
for (const auto& mesh: meshes) {
    if (!mesh.material.diffuseTextures.empty()) {
        auto imageInfo = mesh.material.diffuseTextures[0]->getImageInfo();
        DescriptorWriter(*renderSystemLayout, frameDescriptorPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &imageInfo)
            .build(mesh.descriptorSet);
    }
}
**/
}

void ven::Model::Builder::processMesh(const aiMesh* mesh)
{
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


