find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.doxygen)

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
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${DOXYGEN_OUTPUT_DIRECTORY}/html
            ${CMAKE_SOURCE_DIR}/documentation/html
            VERBATIM
    )
else ()
    message(WARNING "Doxygen not found")
endif ()
