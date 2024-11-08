#include <filesystem>

#include "VEngine/Factories/Texture.hpp"
#include "VEngine/Utils/Logger.hpp"

std::unordered_map<std::string, std::shared_ptr<ven::Texture>> ven::TextureFactory::loadAll(Device& device, const std::string& folderPath)
{
    std::unordered_map<std::string, std::shared_ptr<Texture>> modelCache;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            Logger::logExecutionTime("Creating texture " + entry.path().string(), [&]() {
                const std::string &filepath = entry.path().string();
                modelCache[filepath] = create(device, filepath);
            });
        } else {
            Logger::logWarning("Skipping non-regular file " + entry.path().string());
        }
    }
    return modelCache;
}
