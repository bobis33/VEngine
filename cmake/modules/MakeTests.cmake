if (BUILD_TESTS)
    file(GLOB_RECURSE SOURCES_TESTS ${CMAKE_SOURCE_DIR}/tests/src/*.cpp)
    
    add_executable(${PROJECT_NAME}_tests ${SOURCES_TESTS} ${CMAKE_SOURCE_DIR}/src/Utils/parser.cpp)

    target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${THIRDPARTY_LIBRARIES} gtest gtest_main)
    target_include_directories(${PROJECT_NAME}_tests PRIVATE ${gtest_SOURCE_DIR}/googletest/include ${INCLUDE_DIR})

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_tests)
endif ()