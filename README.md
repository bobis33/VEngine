![VENGINE_LOGO](https://raw.githubusercontent.com/bobis33/VEngine/main/assets/icons/icon.png)

[![Build Linux](https://github.com/bobis33/VEngine/actions/workflows/build-linux.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/build-linux.yml)
[![Build Windows](https://github.com/bobis33/VEngine/actions/workflows/build-windows.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/build-windows.yml)
[![Tests](https://github.com/bobis33/VEngine/actions/workflows/tests.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/tests.yml)
[![Build Documentation](https://github.com/bobis33/VEngine/actions/workflows/update-documentation.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/update-documentation.yml)
![GitHub repo size](https://img.shields.io/github/repo-size/bobis33/VEngine)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/bobis33/VEngine/blob/main/LICENSE)


# VEngine - Vulkan based Game Engine

**WORK IN PROGRESS!**

Welcome to **VEngine**, a Vulkan-based game engine.

I Build this project to learn more about Vulkan and graphics programming in general.
The goal is to create an efficient engine that can be used for various projects, such as games, simulations, and visualizations.


## Features

- **Vulkan Rendering Pipeline**: Leveraging Vulkan for high-performance graphics rendering
- **Basic Camera System**: Control camera movement in the 3D space
- **Model Loading**: Import 3D models using [assimp](https://github.com/assimp/assimp)
- **Real-time debugging**: Use ImGui for real-time debugging and tool development
- **Cross-platform support** (Linux, Windows)
- **Doxygen Documentation**: Automatically generated documentation hosted on [GitHub Pages](https://bobis33.github.io/VEngine/)


## Planned Features:

- **Ray Tracing**
- **Physics Integration**
- **Audio Integration**
- Support for more **input devices** (e.g., mouse, game controller)
- Support for more **platforms** (e.g., macOS, Android, iOS, PS5 ...)


## Build

Before building the project, make sure you update the submodules:

```bash
git submodule update --init --recursive
```

### Prerequisites

Make sure you have the following dependencies installed on your system:

- [CMake 3.27](https://cmake.org/)
- [C++20](https://en.cppreference.com/w/cpp/20)
- [Vulkan SDK](https://www.vulkan.org/)
- [X11](https://www.x.org/wiki/) (Linux only)
- [LLVM](https://llvm.org/)

If you are using a Debian-based distribution, you can install the dependencies using the following command:

```bash
./tools/install-dependencies.sh build
```


### Linux

#### Build and Run

```bash
./tools/build.sh build
```
> This script also handle several other commands: `tests`, `format` and `doc`.

Then you can run the engine:

```bash
./vengine [options]
```


### Windows


#### Build and Run

I build the project with [CLion](https://www.jetbrains.com/clion/), also works with [Visual Studio](https://visualstudio.microsoft.com/fr/).
I'm using the Visual studio toolchain with [ninja](https://ninja-build.org/).

You should create your own CMake profile depending on your configuration.
Basic configuration should run the following commands:

```powershell
cmake -G "Visual Studio 17 2022" .
cmake --build . --config Release 
```

Then you can run the engine:

```powershell
.\vengine.exe [options]
```


### Command Line Options

The following command-line options are available:

| Option               | Description                              |
|:---------------------|:-----------------------------------------|
| `--help`, `-h`       | Show help message and exit               |
| `--version`, `-v`    | Show version information and exit        |
| `--fullscreen`, `-f` | Enable fullscreen mode                   |
| `--vsync`, `-V`      | Enable vertical sync                     |
| `--width <value>`    | Set the width of the window (e.g., 800)  |
| `--height <value>`   | Set the height of the window (e.g., 600) |
| `--fov <value>`      | Set the field of view (1.0 to 300.0)     |
| `--mspeed <value>`   | Set the move speed (0.1 to 100.0)        |
| `--lspeed <value>`   | Set the look speed (0.1 to 100.0)        |
| `--near <value>`     | Set the near plane (0.1 to 100.0)        |
| `--far <value>`      | Set the far plane (0.1 to 100.0)         |


## Key Bindings

The following keyboard controls are currently available for interacting with the engine:

| Key     | Description        |
|:--------|:-------------------|
| `Z`     | Move forward       |
| `S`     | Move backward      |
| `Q`     | Move left          |
| `D`     | Move right         |
| `SHIFT` | Move down          |
| `SPACE` | Move up            |
| `↑`     | Look up            |
| `↓`     | Look down          |
| `←`     | Look left          |
| `→`     | Look right         |
| `à`     | Show debug windows |


## Documentation

The documentation is generated using [Doxygen](https://www.doxygen.nl/index.html).
You can access the latest version on the [GitHub Pages](https://bobis33.github.io/VEngine/).

To generate the documentation locally, run the following command:

```bash
./tools/build.sh doc
```

The generated documentation will be available in the `docs` directory.

You can also run the following command to host the documentation locally:

```bash
./tools/run-doc.sh [ node | php | python ]
```

Then you can access the documentation at [http://localhost:8080](http://localhost:8080).


## External Libraries

- [**Assimp**](https://github.com/assimp/assimp): Open Asset Import Library to load various 3D model formats into the engine.
- [**Doxygen Awesome CSS**](https://github.com/jothepro/doxygen-awesome-css): A custom CSS theme for Doxygen documentation.
- [**GLFW**](https://github.com/glfw/glfw): For creating windows, receiving input, and managing OpenGL and Vulkan contexts.
- [**GLM**](https://github.com/g-truc/glm): A header-only C++ mathematics library for 3D transformations, vectors, and matrices, compatible with OpenGL and Vulkan.
- [**Google Test**](https://github.com/google/googletest): A testing framework for C++.
- [**ImGui**](https://github.com/ocornut/imgui): Immediate Mode Graphical User Interface for real-time debugging and tool development.
- [**stb**](https://github.com/nothings/stb): A set of single-file public domain libraries for graphics, image loading, and more.

These libraries are included directly into the project to simplify dependency management. Be sure to initialize and update the submodules when cloning the repository:


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

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bobis33/VEngine/blob/main/LICENSE.md) file for details.


## Acknowledgements

Special thanks to [Brendan Galea](https://github.com/blurrypiano/littleVulkanEngine) for inspiration and resources related to Vulkan development. His [YouTube channel](https://www.youtube.com/c/BrendanGalea) is a great resource for learning Vulkan and graphics programming.

Also, thanks to [Sascha Willems](https://github.com/SaschaWillems/Vulkan/tree/master) for his excellent Vulkan examples and tutorials.

Without forgetting [Beaumanvienna](https://github.com/beaumanvienna/vulkan) for his Vulkan tutorial series on YouTube, and his precious help.


