find_package(Doxygen REQUIRED)

set(DOXYGEN_DIR ${CMAKE_SOURCE_DIR}/documentation/.doxygen)
set(DOXYFILE_IN ${DOXYGEN_DIR}/Doxyfile)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

doxygen_add_docs(doxygen
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/assets/shaders
)

add_custom_command(TARGET doxygen POST_BUILD
        WORKING_DIRECTORY ${DOXYGEN_DIR}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        VERBATIM
)

add_custom_command(TARGET doxygen POST_BUILD
        WORKING_DIRECTORY ${DOXYGEN_DIR}/latex
        COMMAND ${CMAKE_MAKE_PROGRAM} > /dev/null && ${CMAKE_COMMAND} -E copy refman.pdf ${CMAKE_SOURCE_DIR}/documentation/VEngine.pdf
        BYPRODUCTS ${CMAKE_SOURCE_DIR}/documentation/VEngine.pdf
        VERBATIM
)
