add_library(thirdparty INTERFACE)

#======================================= Vulkan ======================================#
find_package(Vulkan REQUIRED)
list(APPEND THIRDPARTY_LIBS Vulkan::Vulkan)
list(APPEND THIRDPARTY_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS})

#======================================= Assimp ======================================#
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(assimp)
list(APPEND THIRDPARTY_LIBS assimp)
list(APPEND THIRDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)

#======================================= GLFW =======================================#
add_subdirectory(glfw)
list(APPEND THIRDPARTY_LIBS glfw)
list(APPEND THIRDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)

#======================================= GLM =======================================#
list(APPEND THIRDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glm)

#======================================= GoogleTest =======================================#
if (BUILD_TESTS)
    add_subdirectory(googletest)
endif()

#======================================= ImGui =======================================#
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
        ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
        ${Vulkan_INCLUDE_DIRS}
)

list(APPEND THIRDPARTY_LIBS imgui)
list(APPEND THIRDPARTY_INCLUDE_DIRS ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(thirdparty INTERFACE ${THIRDPARTY_LIBS})
target_include_directories(thirdparty SYSTEM INTERFACE ${THIRDPARTY_INCLUDE_DIRS})

set(THIRDPARTY_LIBRARIES thirdparty PARENT_SCOPE)
