set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SHADER_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${SHADER_BIN_DIR})

file(GLOB VERTEX_SHADERS ${SHADER_SRC_DIR}/*.vert)
file(GLOB FRAGMENT_SHADERS ${SHADER_SRC_DIR}/*.frag)

foreach(SHADER ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME_WE)
    set(SPIRV_BINARY ${SHADER_BIN_DIR}/${FILE_NAME}.spv)
    add_custom_command(
            OUTPUT ${SPIRV_BINARY}
            COMMAND glslangValidator -V ${SHADER} -o ${SPIRV_BINARY}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILE_NAME}"
            VERBATIM
    )
    list(APPEND SPIRV_BINARIES ${SPIRV_BINARY})
endforeach()

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARIES}
)
