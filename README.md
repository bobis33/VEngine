![VENGINE_LOGO](https://raw.githubusercontent.com/bobis33/VEngine/main/assets/logo.png)

[![VENGINE_CI](https://github.com/bobis33/VEngine/actions/workflows/check-compilation.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/check-compilation.yml)
[![VENGINE_CI](https://github.com/bobis33/VEngine/actions/workflows/update-adocumentation.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/update-documentation.yml)
![GitHub repo size](https://img.shields.io/github/repo-size/bobis33/VEngine)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/bobis33/VEngine/blob/main/LICENSE)


# VEngine - Vulkan Graphics Engine

**WORK IN PROGRESS!**

Welcome to **VEngine**, a Vulkan-based graphics engine.

This project is designed to provide a high-performance and flexible foundation for building 3D applications and games, 
taking full advantage of the Vulkan API.


## Features

- **Vulkan Rendering Pipeline**: Leveraging Vulkan for high-performance graphics rendering
- **Basic Camera System**: Control camera movement in the 3D space
- **Input System**: Keyboard-based controls for movement and looking around
- **Model Loading**: Import 3D models using TinyObjLoader
- **Real-time debugging**: Toggle debug windows using key bindings
- **Doxygen Documentation**: Automatically generated documentation hosted on GitHub Pages


### Planned Features:

- **Cross-platform support** (Linux, macOS, Windows)
- Improve shadow
- Model Importing (using Assimp)
- Physics Integration
- Support for more input devices (e.g., mouse, game controller)
- Audio Integration


## Prerequisites

Make sure you have the following dependencies installed on your system:

- [CMake 3.27](https://cmake.org/)
- [C++20](https://en.cppreference.com/w/cpp/20)
- [Vulkan](https://www.vulkan.org/)
- [GLM](https://github.com/g-truc/glm)
- [assimp](https://www.assimp.org/) (unused ATM)


## Usage

### Build

```bash
$> ./scripts/build.sh build
[...]
```
> This script also handle several other commands: `clean`, `format` and `doc`.


### Run

```bash
$> ./vengine
[...]
```


## Key Bindings

The following keyboard controls are currently available for interacting with the engine:

| Key           | Description                                                                 |
|:--------------|:----------------------------------------------------------------------------|
| `z`           | Move forward                                                                |
| `S`           | Move backward                                                               |
| `q`           | Move left                                                                   |
| `D`           | Move right                                                                  |
| `SHIFT`       | Move down                                                                   |
| `SPACE`       | Move up                                                                     |
| `arrow up`    | Look up                                                                     |
| `arrow down`  | Look down                                                                   |
| `arrow left`  | Look left                                                                   |
| `arrow right` | Look right                                                                  |
| `F1`          | Show debug windows                                                          |


## Documentation

The documentation is generated using [Doxygen](https://www.doxygen.nl/index.html).
You can access the latest version on the [GitHub Pages dite](https://bobis33.github.io/VEngine/).


## Commit Norms

| Commit Type | Description                                                                                                               |
|:------------|:--------------------------------------------------------------------------------------------------------------------------|
| build       | Changes that affect the build system or external dependencies (npm, make, etc.)                                           |
| ci          | Changes related to integration files and scripts or configuration (Travis, Ansible, BrowserStack, etc.)                   |
| feat        | Addition of a new feature                                                                                                 |
| fix         | Bug fix                                                                                                                   |
| perf        | Performance improvements                                                                                                  |
| refactor    | Modification that neither adds a new feature nor improves performance                                                     |
| style       | Change that does not affect functionality or semantics (indentation, formatting, adding space, renaming a variable, etc.) |
| docs        | Writing or updating documentation                                                                                         |
| test        | Addition or modification of tests                                                                                         |


## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bobis33/VEngine/blob/main/LICENSE) file for details.


## Acknowledgements

Special thanks to [Brendan Galea](https://github.com/blurrypiano/littleVulkanEngine) for inspiration and resources related to Vulkan development.
