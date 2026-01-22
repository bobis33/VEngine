#pragma once

#include <stdexcept>
#include <string>

namespace utl {

    constexpr auto THROW_ERROR = [](const char* msg) {
        return std::runtime_error(std::string(msg) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");
    };

    void printError(const std::string& msg);

} // namespace utl
