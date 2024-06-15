/*
** @file Shader.hpp
** @brief This file contains the Shader class
** @namespace ven
*/

#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <string>

namespace ven {

    class Shader {

        public:

            Shader() = default;
            ~Shader() = default;

            static VkShaderModule createShaderModule(VkDevice device, const std::vector<char> &code);
            void createGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath);

        private:

            static std::vector<char> readFile(const std::string &filename);

    }; // class VenPipeline

} // namespace ven
