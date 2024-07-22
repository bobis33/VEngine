find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.doxygen)

    set(doxyfile_in ${CMAKE_SOURCE_DIR}/docs/doxygen/Doxyfile)
    set(doxyfile_out ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile_out} @ONLY)

    doxygen_add_docs(doc
            COMMENT "Generating API documentation for VEngine"
            ${CMAKE_SOURCE_DIR}/lib/local/static/myLib/include
            ${CMAKE_SOURCE_DIR}/include
    )

    add_custom_command(TARGET doc
            POST_BUILD
            WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/latex
            COMMAND ${CMAKE_MAKE_PROGRAM} > /dev/null && ${CMAKE_COMMAND} -E copy
            ${DOXYGEN_OUTPUT_DIRECTORY}/latex/refman.pdf
            ${CMAKE_SOURCE_DIR}/docs/VEngine.pdf
            BYPRODUCTS ${CMAKE_SOURCE_DIR}/docs/VEngine.pdf
            VERBATIM
    )
    add_custom_command(TARGET doc
            POST_BUILD
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_out}
            WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
            COMMENT "Generating HTML documentation with Doxygen"
            VERBATIM)
else ()
    message(WARNING "Doxygen not found")
endif ()
