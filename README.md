# VENGINE

<img src="doc/vulkan_logo.png" alt="logo">

[![VENGINE](https://github.com/bobis33/VEngine/actions/workflows/VEngine.yml/badge.svg)](https://github.com/bobis33/VEngine/actions/workflows/VEngine.yml)

ACTUALLY WORKING ON IT!

## Description

This is a simple game engine that I am developing for fun. It is written in C++ and uses Vulkan for rendering.


## Pre-requisites

- CMake 3.27
- C++20
- Vulkan SDK
- GLFW3
- GLM


## Usage

### Build

```bash
$> ./build.sh build
[...]
```
> if it doesn't work, try the following command:
```bash
$> ./build.sh clean && ./build.sh build
[...]
```

### Run

```bash
$> ./vengine
[...]
```


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
