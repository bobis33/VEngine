#include <iostream>

#include "VEngine/Core/Engine.hpp"

using namespace ven;

int main()
{
    try {
        Engine engine{};
        engine.mainLoop();
        Engine::cleanup();
    } catch (const std::exception &e) {
        std::cerr << "std exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}
