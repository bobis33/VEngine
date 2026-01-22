///
/// @file Utils.hpp
/// @brief This file contains utility functions
/// @namespace utl
///

#pragma once

#include <string>
#include <vector>

namespace utl {

      [[nodiscard]] std::vector<char> readFile(const std::string& filename);

} // namespace utl
