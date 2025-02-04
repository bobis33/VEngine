#include "VEngine/Gfx/Mesh.hpp"

std::vector<VkVertexInputBindingDescription> ven::Vertex::getBindingDescriptions()
{
    return {{.binding=0, .stride=sizeof(Vertex), .inputRate=VK_VERTEX_INPUT_RATE_VERTEX}};
}

std::vector<VkVertexInputAttributeDescription> ven::Vertex::getAttributeDescriptions()
{
    return {
        {.location=0, .binding=0, .format=VK_FORMAT_R32G32B32_SFLOAT, .offset=offsetof(Vertex, position)},
        {.location=1, .binding=0, .format=VK_FORMAT_R32G32B32_SFLOAT, .offset=offsetof(Vertex, color)},
        {.location=2, .binding=0, .format=VK_FORMAT_R32G32B32_SFLOAT, .offset=offsetof(Vertex, normal)},
        {.location=3, .binding=0, .format=VK_FORMAT_R32G32_SFLOAT, .offset=offsetof(Vertex, uv)}
    };
}
