if (NOT USE_CLANG_TIDY)
    return()
endif()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

if (CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-header-filter=.*")
    set_source_files_properties(${SOURCES} PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
else()
    message(FATAL_ERROR "clang-tidy not found")
endif()
