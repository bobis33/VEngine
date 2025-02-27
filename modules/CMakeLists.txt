add_library(modules INTERFACE)

add_subdirectory(Utils)
list(APPEND MODULES_LIBS utils)
list(APPEND MODULES_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Utils/include)

target_link_libraries(modules INTERFACE ${MODULES_LIBS})
target_include_directories(modules SYSTEM INTERFACE ${MODULES_INCLUDE_DIRS})

set(MODULES_LIBRARIES modules PARENT_SCOPE)
