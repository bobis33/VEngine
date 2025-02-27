#======================================= Options ======================================#
option(USE_CLANG_TIDY "Use Clang-tidy" OFF)
option(BUILD_DOC "Build documentation only" OFF)
option(BUILD_TESTS "Build tests" OFF)

#======================================= Variables ======================================#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

SET(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(THIRDPARTY_LIBRARIES "")
set(MODULES_LIBRARIES "")
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders)
set(SHADER_BIN_DIR ${CMAKE_BINARY_DIR}/shaders)

## todo(bobis33): to fix, check which warning can be used with which compiler and which version
if (NOT WIN32)
	set(WARNING_FLAG
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
endif()

if(WIN32)
	set(WARNING_FLAG -Wno-error)
endif()

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)
