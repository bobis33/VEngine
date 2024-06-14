set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/plugins/)

set(SRC_DIR
        ${CMAKE_SOURCE_DIR}/src/
)

SET(INCLUDE_DIR
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/lib/static/myLib/include/
)

set(WARNING_FLAGS
        -Wall
        -Wextra
        -Wdeprecated-copy
        -Wmisleading-indentation
        -Wnull-dereference
        -Woverloaded-virtual
        -Wpedantic
        -Wshadow
        -Wsign-conversion
        -Wnon-virtual-dtor
        -Wunused
        -Wcast-align
        -Wno-padded
        -Wconversion
        -Wformat
        -Winit-self
        -Wmissing-include-dirs
        -Wold-style-cast
        -Wredundant-decls
        -Wswitch-default
        -Wundef
)