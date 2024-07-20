find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_LATEX YES)
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
            COMMAND ${CMAKE_MAKE_PROGRAM} > /dev/null
            && ${CMAKE_COMMAND} -E copy
            ${DOXYGEN_OUTPUT_DIRECTORY}/latex/refman.pdf
            ${CMAKE_SOURCE_DIR}/documentation/VEngine.pdf
            BYPRODUCTS ${CMAKE_SOURCE_DIR}/documentation/VEngine.pdf
            VERBATIM
    )
else ()
    message(FATAL_ERROR "Doxygen is necessary for docs")
endif ()
