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
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found")
endif()

option(BUILD_EXTERNAL_LIBRARIES "Build external libraries (like GLFW)" ON)
option(USE_CLANG_TIDY "Use Clang-tidy" OFF)

include(${CMAKE_MODULE_PATH}/myVariables.cmake)
include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
include(${CMAKE_MODULE_PATH}/MakeDoc.cmake)
if (USE_CLANG_TIDY)
    include(${CMAKE_MODULE_PATH}/ClangTidy.cmake)
endif()

clangformat_setup()

# Add shaders directory
add_subdirectory(shaders)

# Add libraries directory
add_subdirectory(lib)

file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)

# Main executable
add_executable(${PROJECT_NAME} ${SRCS})

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
