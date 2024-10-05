#include <iostream>

#include "VEngine/Engine.hpp"

using namespace ven;

int main()
{
    try {
        Engine engine{};
        engine.mainLoop();
    } catch (const std::exception &e) {
        std::cerr << "std exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}
