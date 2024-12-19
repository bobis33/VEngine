///
/// @file Model.hpp
/// @brief This file contains the Model Factory class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Gfx/Model.hpp"

namespace ven {

    ///
    /// @class ModelFactory
    /// @brief Class for Model factory
    /// @namespace ven
    ///
    class ModelFactory {

        public:

            ModelFactory() = delete;
            ~ModelFactory() = default;

            ModelFactory(const ModelFactory&) = delete;
            ModelFactory& operator=(const ModelFactory&) = delete;
            ModelFactory(ModelFactory&&) = delete;
            ModelFactory& operator=(ModelFactory&&) = delete;

            static std::unique_ptr<Model> get(Device& device, const std::string& filepath);
            static std::unordered_map<std::string, std::shared_ptr<Model>> getAll(Device& device, const std::string& folderPath);

    }; // class ModelFactory

} // namespace ven
