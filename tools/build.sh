#!/bin/bash

CMAKE_CMD=(cmake -S . -Bbuild -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release)

case $1 in
    build)
        "${CMAKE_CMD[@]}" && cmake --build build --parallel 4
        ;;
    format)
        "${CMAKE_CMD[@]}" -DUSE_CLANG_TIDY=ON && cmake --build build --target clangformat
        ;;
    tests)
        "${CMAKE_CMD[@]}" -DBUILD_TESTS=ON && cmake --build build
        ;;
    doc)
        "${CMAKE_CMD[@]}" -DBUILD_DOC=ON && cmake --build build --target doxygen
        ;;
    *)
        echo "[ERROR] Invalid command. Usage: $0 build | format | tests | doc"
        exit 1
        ;;
esac
