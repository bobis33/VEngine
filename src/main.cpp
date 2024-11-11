#include <iostream>

#include "VEngine/Core/Engine.hpp"
#include "VEngine/Utils/Parser.hpp"

using namespace ven;

int main(const int argc, char *argv[], char *envp[])
{
    try {
        Logger::getInstance();
        Engine(Parser(argc, argv, envp).getConfig()).mainLoop();
    } catch (const ParserException &e) {
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
