cmake_minimum_required(VERSION 3.27)

project(vengine
        VERSION 0.0.1
        DESCRIPTION "3D graphics engine"
        LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

configure_file(${CMAKE_SOURCE_DIR}/cmake/config/Version.hpp.in ${CMAKE_SOURCE_DIR}/include/VEngine/Generated/Version.hpp)

include(PathsAndOptions)
if (BUILD_DOC)
    include(MakeDoc)
    return()
endif()
include(ClangTidy)
include(ClangFormat)
clangformat_setup()

include(MakeShaders)

add_subdirectory(third-party)

if (BUILD_TESTS)
    include(MakeTests)
endif ()

add_executable(${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} thirdparty shaders)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${THIRDPARTY_LIBRARIES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
