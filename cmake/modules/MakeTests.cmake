SET(BINARY_NAME_TESTS ${PROJECT_NAME}-tests)

file(GLOB_RECURSE SOURCES_TESTS ${CMAKE_SOURCE_DIR}/tests/src/*.cpp)
    
add_executable(${BINARY_NAME_TESTS} ${SOURCES_TESTS} ${CMAKE_SOURCE_DIR}/src/Utils/parser.cpp)

target_link_libraries(${BINARY_NAME_TESTS} PRIVATE ${THIRDPARTY_LIBRARIES} gtest gtest_main)
target_include_directories(${BINARY_NAME_TESTS} PRIVATE ${gtest_SOURCE_DIR}/googletest/include ${INCLUDE_DIR})

include(GoogleTest)
gtest_discover_tests(${BINARY_NAME_TESTS})
