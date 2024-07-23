function(prefix_clangformat_setup prefix)
    if(NOT CLANGFORMAT_EXECUTABLE)
        find_program(CLANGFORMAT_EXECUTABLE NAMES "clang-format" REQUIRED)
    endif()

    set(clangformat_sources)
    foreach(clangformat_source ${ARGN})
        get_filename_component(clangformat_source ${clangformat_source} ABSOLUTE)
        list(APPEND clangformat_sources ${clangformat_source})
    endforeach()

    add_custom_target(${prefix}_clangformat
            COMMAND ${CLANGFORMAT_EXECUTABLE} -style=file -i ${clangformat_sources}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting ${prefix} with ${CLANGFORMAT_EXECUTABLE} ..."
    )

    if(NOT TARGET clangformat)
        add_custom_target(clangformat)
    endif()
    add_dependencies(clangformat ${prefix}_clangformat)
endfunction()

function(clangformat_setup)
    file(GLOB_RECURSE ALL_FILES
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/include/VEngine/*.hpp"
    )
    prefix_clangformat_setup(${PROJECT_NAME} ${ALL_FILES})
endfunction()

function(target_clangformat_setup target)
    get_target_property(target_sources ${target} SOURCES)
    prefix_clangformat_setup(${target} ${target_sources})
endfunction()
