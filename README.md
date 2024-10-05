![VENGINE_LOGO](https://raw.githubusercontent.com/bobis33/VEngine/main/assets/logo.png)

[![VENGINE_CI](https://github.com/bobis33/VEngine/actions/workflows/check_compilation.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/check_compilation.yml)
[![VENGINE_CI](https://github.com/bobis33/VEngine/actions/workflows/update_documentation.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/update_documentation.yml)
![GitHub repo size](https://img.shields.io/github/repo-size/bobis33/VEngine)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/bobis33/VEngine/blob/main/LICENSE)


# Description

**ACTUALLY WORKING ON IT!**

Welcome to **VEngine**, a graphics engine developed with Vulkan.
This project aims to provide a robust foundation for game and application developers, focusing on the performance and flexibility offered by Vulkan.


# Prerequisites

- [CMake 3.27](https://cmake.org/)
- [C++20](https://en.cppreference.com/w/cpp/20)
- [Vulkan](https://www.vulkan.org/)
- [GLM](https://github.com/g-truc/glm)
- [assimp](https://www.assimp.org/) (unused ATM)


# Usage

## Build

```bash
$> ./build.sh build
[...]
```
> This script also handle several other commands: `clean`, `format` and `doc`.


## Run

```bash
$> ./vengine
[...]
```


## Key Bindings

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
You can visualize it on the [GitHub Pages](https://bobis33.github.io/VEngine/).


# Commit Norms

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


# License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bobis33/VEngine/blob/main/LICENSE) file for details.


# Acknowledgements

Thanks to [Brendan Galea](https://github.com/blurrypiano/littleVulkanEngine).
