#include "VEngine/Factories/Model.hpp"

std::unordered_map<std::string, std::shared_ptr<ven::Model>> ven::ModelFactory::loadAll(Device& device, const std::string& folderPath)
{
    std::unordered_map<std::string, std::shared_ptr<Model>> modelCache;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            Logger::logExecutionTime("Creating model " + entry.path().string(), [&]() {
                const std::string &filepath = entry.path().string();
                modelCache[filepath] = create(device, filepath);
            });
        } else {
            Logger::logWarning("Skipping non-regular file " + entry.path().string());
        }
    }
    return modelCache;
}

std::unique_ptr<ven::Model> ven::ModelFactory::create(Device& device, const std::string& filepath)
{
    Model::Builder builder{};
    builder.loadModel(filepath);
    return std::make_unique<Model>(device, builder);
}
