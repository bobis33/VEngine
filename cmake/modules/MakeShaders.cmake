file(MAKE_DIRECTORY ${SHADER_BIN_DIR})

file(GLOB SHADERS ${SHADER_SRC_DIR}/*.vert ${SHADER_SRC_DIR}/*.frag)

foreach(SHADER ${SHADERS})
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

add_custom_target(shaders DEPENDS ${SPIRV_BINARIES})
