## assimp
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(assimp)

## glfw
add_subdirectory(glfw)

## Add imgui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SRCS
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

add_library(imgui STATIC ${IMGUI_SRCS})
target_include_directories(imgui PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
