cmake_minimum_required(VERSION 3.27)

project(vengine
        DESCRIPTION "VENGINE"
        LANGUAGES CXX
        VERSION 0.1.0
)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

# Vulkan
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Vulkan not found")
endif()

option(BUILD_EXTERNAL_LIBRARIES "Build external libraries (like GLFW)" ON)
option(USE_CLANG_TIDY "Use Clang-tidy" OFF)

include(${CMAKE_MODULE_PATH}/myVariables.cmake)

if (USE_CLANG_TIDY)
    include(${CMAKE_MODULE_PATH}/ClangTidy.cmake)
endif()

include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
clangformat_setup()

include(${CMAKE_MODULE_PATH}/MakeDoc.cmake)

# Add External libraries
if (BUILD_EXTERNAL_LIBRARIES)
    add_subdirectory(${EXTERNAL_DIR}/glfw)
endif()
add_subdirectory(${EXTERNAL_DIR}/tinyobjloader)

include_directories(${Vulkan_INCLUDE_DIRS} ${EXTERNAL_DIR}/glfw/include ${EXTERNAL_DIR}/tinyobjloader)

# Add shaders directory
add_subdirectory(shaders)

# Libraries
add_subdirectory(lib)

# Main executable
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/engine.cpp
        ${SRC_DIR}/window.cpp
        ${SRC_DIR}/device.cpp
        ${SRC_DIR}/shaders.cpp
        ${SRC_DIR}/swapChain.cpp
        ${SRC_DIR}/model.cpp
        ${SRC_DIR}/renderer.cpp
        ${SRC_DIR}/system/renderSystem.cpp
        ${SRC_DIR}/system/pointLightSystem.cpp
        ${SRC_DIR}/camera.cpp
        ${SRC_DIR}/keyboardController.cpp
        ${SRC_DIR}/object.cpp
        ${SRC_DIR}/buffer.cpp
        ${SRC_DIR}/descriptors.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE my_lib glfw tinyobjloader ${Vulkan_LIBRARIES})

add_dependencies(${PROJECT_NAME} Shaders)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
