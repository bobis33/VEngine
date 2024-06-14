#include <dlfcn.h>
#include <filesystem>

#include "VEngine/PluginLoader.hpp"


/*
void gui::PluginLoader::closePlugins()
{
    for (void* handle : m_handles) {
        if (handle != nullptr) {
            dlclose(handle);
        }
    }
}

void gui::PluginLoader::loadPlugins()
{
    void *handle = nullptr;

    for (const auto& entry : std::filesystem::directory_iterator("./plugins")) {
        if (entry.path().extension() == ".so") {
            handle = dlopen(entry.path().c_str(), RTLD_LAZY);
            if (handle == nullptr) {
                throw PluginLoaderException(dlerror());
            }
            m_handles.emplace_back(handle);
            auto creator = reinterpret_cast<PluginCreator>(dlsym(handle, "entryPoint"));
            if (creator == nullptr) {
                dlclose(handle);
                throw PluginLoaderException(dlerror());
            }
            m_plugins.emplace(creator()->getPluginName(), creator);
        }
    }
}

template <typename T>
std::unique_ptr<T> gui::PluginLoader::getPlugin(const std::string &pluginName) {
    auto plugin_iterator = m_plugins.find(pluginName);
    if (plugin_iterator == m_plugins.end()) {
        throw PluginLoaderException("Plugin not found");
    }
    PluginCreator creator = plugin_iterator->second;
    if constexpr (std::is_base_of_v<IPlugin, T>) {
        return std::unique_ptr<T>(static_cast<T*>(creator().release()));
    } else {
        throw PluginLoaderException("Invalid plugin type");
    }
}
template std::unique_ptr<gui::IRenderer> gui::PluginLoader::getPlugin<gui::IRenderer>(const std::string &pluginName);
*/