#include <stdexcept>
#include <fstream>
#include <iostream>

#include "VEngine/Shader.hpp"

std::vector<char> ven::Shader::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    std::streamsize fileSize = file.tellg();
    std::vector<char> buffer(static_cast<unsigned long>(fileSize));

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

VkShaderModule ven::Shader::createShaderModule(VkDevice device, const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule = nullptr;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}

void ven::Shader::createGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath)
{
    auto vertCode = readFile(vertexFilePath);
    auto fragCode = readFile(fragmentFilePath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}
