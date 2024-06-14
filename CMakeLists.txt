cmake_minimum_required(VERSION 3.27)

project(vengine
        DESCRIPTION "VENGINE"
        LANGUAGES CXX
        VERSION 0.1.0
)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

option(USE_CLANG_TIDY "Use Clang-tidy" OFF)

include(${CMAKE_MODULE_PATH}/myVariables.cmake)

# Build Doxygen documentation
include(${CMAKE_MODULE_PATH}/MakeDoc.cmake)

# Clang format
if (USE_CLANG_TIDY)
    include(${CMAKE_MODULE_PATH}/ClangTidy.cmake)
endif()

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# Libraries
add_subdirectory(lib)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_sources(${PROJECT_NAME} PRIVATE
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/engine.cpp
        ${SRC_DIR}/window.cpp
        ${SRC_DIR}/device.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE my_lib glfw ${Vulkan_LIBRARIES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
