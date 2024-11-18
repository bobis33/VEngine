///
/// @file Message.hpp
/// @brief
/// @namespace ven
///

#pragma once

#include "VEngine/Generated/Version.hpp"

namespace ven {

    constexpr auto VERSION_MESSAGE = "VEngine Version " PROJECT_VERSION "\n"
          "Built on " __DATE__
          " at " __TIME__ "\n"
          "Author: Elliot Masina (masina.elliot@hotmail.fr)\n"
          "License: MIT\n"
          "Repository: https://github.com/bobis33/VEngine\n"
          "Documentation: https://bobis33.github.io/VEngine\n";

    constexpr auto HELP_MESSAGE = "Usage: VEngine [options]\n"
          "Options:\n"
          "  --help, -h           Show this help message and exit\n"
          "  --version, -v        Show version information and exit\n"
          "  --fullscreen, -f     Enable fullscreen mode\n"
          "  --vsync, -V          Enable vertical sync\n"
          "  --width <value>      Set the width of the window (e.g., 800)\n"
          "  --height <value>     Set the height of the window (e.g., 600)\n"
          "  --fov <value>        Set the field of view (1.0 to 300.0)\n"
          "  --mspeed <value>     Set the move speed (0.1 to 100.0)\n"
          "  --lspeed <value>     Set the look speed (0.1 to 100.0)\n"
          "  --near <value>       Set the near plane (0.1 to 100.0)\n"
          "  --far <value>        Set the far plane (0.1 to 100.0)\n";

} // namespace ven
