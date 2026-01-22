#!/bin/bash

CMAKE_CMD=(cmake -S . -G "Ninja")

case $1 in
    release)
        "${CMAKE_CMD[@]}" -B cmake-build-release -DCMAKE_BUILD_TYPE=Release && cmake --build cmake-build-release -- -j 4
        ;;
    debug)
        "${CMAKE_CMD[@]}" -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug && cmake --build cmake-build-debug -- -j 4
        ;;
    format)
        "${CMAKE_CMD[@]}" -B cmake-build-format -DUSE_CLANG_TIDY=ON && cmake --build cmake-build-format --target clangformat
        ;;
    tests)
        "${CMAKE_CMD[@]}" -B cmake-build-tests -DBUILD_TESTS=ON && cmake --build cmake-build-tests
        ;;
    doc)
        "${CMAKE_CMD[@]}" -B cmake-build-doc -DBUILD_DOC=ON && cmake --build cmake-build-doc --target doxygen
        ;;
    *)
        echo "[ERROR] Invalid command. Usage: $0 release | debug | tests | format | doc"
        exit 1
        ;;
esac
