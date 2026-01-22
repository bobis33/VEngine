function(copy_directory_to_target TARGET SRC_DIR DST_SUBDIR)
    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "Target '${TARGET}' does not exist")
    endif()

    set(COPY_TARGET_NAME ${TARGET}-copy-assets)

    set(DST_DIR "$<TARGET_FILE_DIR:${TARGET}>/${DST_SUBDIR}")

    add_custom_target(${COPY_TARGET_NAME} ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SRC_DIR}
            ${DST_DIR}
            COMMENT "Copying '${SRC_DIR}' to '${DST_DIR}'"
    )

    add_dependencies(${TARGET} ${COPY_TARGET_NAME})
endfunction()
