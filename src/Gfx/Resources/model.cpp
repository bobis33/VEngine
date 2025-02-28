#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "VEngine/Gfx/Resources/Model.hpp"
#include "VEngine/Gfx/Resources/TextureManager.hpp"

template<> struct std::hash<ven::Vertex> {
    size_t operator()(ven::Vertex const& vertex) const noexcept {
        return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};

glm::mat4 aiMatrixToGlm(const aiMatrix4x4& matrix) {
    glm::mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

glm::mat4 getNodeTransformation(const aiNode* node) {
    return aiMatrixToGlm(node->mTransformation);;
}

ven::Model::Model(const Device& device, const SwapChain& swapChain, const std::string& path)
    : m_device(device), m_swapChain(swapChain) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U || scene->mRootNode == nullptr) {
        throw utl::THROW_ERROR(importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene, device, swapChain, glm::mat4(1.0F));
}

void ven::Model::processNode(const aiNode* node, const aiScene* scene, const Device& device, const SwapChain& swapChain, const glm::mat4 &parentTransform) {
    const glm::mat4 globalTransform = parentTransform * getNodeTransformation(node);
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene, device, swapChain, globalTransform));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, device, swapChain, globalTransform);
    }
}

std::unique_ptr<ven::Mesh> ven::Model::processMesh(const aiMesh* mesh, const aiScene* scene, const Device& device, const SwapChain& swapChain, const glm::mat4& transform) {
    auto newMesh = std::make_unique<Mesh>();
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        vertex.pos = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };
        vertex.color = {1.0F, 1.0F, 1.0F};
        if (mesh->mTextureCoords[0] != nullptr) {
            vertex.texCoord = {
                mesh->mTextureCoords[0][i].x,
                1.0F - mesh->mTextureCoords[0][i].y
            };
        } else {
            vertex.texCoord = {0.0F, 0.0F};
        }
        glm::vec4 transformedPos = transform * glm::vec4(vertex.pos, 1.0f);
        vertex.pos = glm::vec3(transformedPos);
        if (!uniqueVertices.contains(vertex)) {
            uniqueVertices[vertex] = static_cast<uint32_t>(newMesh->getVertices().size());
            newMesh->addVertex(vertex);
        }
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            newMesh->addIndices(uniqueVertices[ Vertex{ .pos = {
                mesh->mVertices[face.mIndices[j]].x,
                mesh->mVertices[face.mIndices[j]].y,
                mesh->mVertices[face.mIndices[j]].z
            }, .color = {1.0F, 1.0F, 1.0F}, .texCoord = mesh->mTextureCoords[0] != nullptr ? glm::vec2{
                mesh->mTextureCoords[0][face.mIndices[j]].x,
                1.0F - mesh->mTextureCoords[0][face.mIndices[j]].y
            } : glm::vec2{0.0F, 0.0F}
            }]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texturePath;
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
            newMesh->setTextureIndex(TextureManager::getTextureIndex("assets/textures/" + std::string(texturePath.C_Str())));
        } else if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
            newMesh->setTextureIndex(TextureManager::getTextureIndex("assets/textures/" + std::string(texturePath.C_Str())));
        } else if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
            material->GetTexture(aiTextureType_NORMALS, 0, &texturePath);
            newMesh->setTextureIndex(TextureManager::getTextureIndex("assets/textures/" + std::string(texturePath.C_Str())));
        } else {
            newMesh->setTextureIndex(TextureManager::getTextureIndex("assets/textures/default.png"));
        }
    }
    return newMesh;
}
