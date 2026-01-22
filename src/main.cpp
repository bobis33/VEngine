#include <optional>
#include <memory>
#include <vector>
#include <unordered_map>

#include <vulkan/vulkan.h>

#include "VEngine/Core/Engine.hpp"

int main() {
    try {
        ven::Engine().run();
    } catch (const std::exception& e) {
        utl::printError(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
