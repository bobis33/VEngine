cmake_minimum_required(VERSION 3.27)

project(vengine
        DESCRIPTION "3D graphics engine made with Vulkan"
        LANGUAGES CXX
        VERSION 0.1.0
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(${CMAKE_MODULE_PATH}/myVariables.cmake)
include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
clangformat_setup()
include(${CMAKE_MODULE_PATH}/MakeDoc.cmake)
if (USE_CLANG_TIDY)
    include(${CMAKE_MODULE_PATH}/ClangTidy.cmake)
endif()

find_package(Vulkan)

if (VULKAN_FOUND)

    file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

    add_subdirectory(third-party)
    add_subdirectory(shaders)

    add_executable(${PROJECT_NAME} ${SOURCES})
    add_dependencies(${PROJECT_NAME} ThirdParty Shaders)

    target_include_directories(${PROJECT_NAME} PRIVATE
            ${INCLUDE_DIR}
            ${Vulkan_INCLUDE_DIRS}
            ${THIRDPARTY_INCLUDE_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
            Vulkan::Vulkan
            ${THIRDPARTY_LIBRARIES}
    )

    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})

else()
    message(WARNING "Vulkan not found")
endif ()
