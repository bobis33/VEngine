#include "VEngine/Factories/Model.hpp"

#include "VEngine/Utils/Logger.hpp"

std::unique_ptr<ven::Model> ven::ModelFactory::get(Device& device, const std::string& filepath)
{
    Model::Builder builder{};
    builder.loadModel(device, filepath);
    return std::make_unique<Model>(device, builder);
}

std::unordered_map<std::string, std::shared_ptr<ven::Model>> ven::ModelFactory::getAll(Device& device, const std::string& folderPath)
{
    std::unordered_map<std::string, std::shared_ptr<Model>> modelCache;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            Logger::logExecutionTime("Creating model " + entry.path().string(), [&]() {
                const std::string &filepath = entry.path().string();
                modelCache[filepath] = get(device, filepath);
            });
        } else {
            Logger::logWarning("Skipping non-regular file " + entry.path().string());
        }
    }
    return modelCache;
}
