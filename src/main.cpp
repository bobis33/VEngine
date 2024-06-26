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
        return VEN_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return VEN_FAILURE;
    }
    return VEN_SUCCESS;
}
