cmake_minimum_required(VERSION 3.27)

project(vengine
        DESCRIPTION "3D graphics engine made with Vulkan"
        LANGUAGES CXX
        VERSION 0.1.0
)

# Vulkan
find_package(Vulkan REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

option(BUILD_EXTERNAL_LIBRARIES "Build external libraries (like GLFW)" ON)
option(USE_CLANG_TIDY "Use Clang-tidy" OFF)

include(${CMAKE_MODULE_PATH}/myVariables.cmake)
include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
clangformat_setup()
include(${CMAKE_MODULE_PATH}/MakeDoc.cmake)
if (USE_CLANG_TIDY)
    include(${CMAKE_MODULE_PATH}/ClangTidy.cmake)
endif()

# Add shaders directory
add_subdirectory(shaders)

# Add libraries directory
add_subdirectory(lib)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${INCLUDE_DIR}
        ${Vulkan_INCLUDE_DIRS}
        ${EXTERNAL_DIR}/glfw/include
        ${EXTERNAL_DIR}/tinyobjloader
        ${EXTERNAL_DIR}/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        my_lib
        ${Vulkan_LIBRARIES}
        glfw
        tinyobjloader
)

add_dependencies(${PROJECT_NAME} Shaders)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
