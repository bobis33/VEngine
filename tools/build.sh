#!/bin/bash

CMAKE_CMD=(cmake -S . -Bbuild -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release)

function log() {
    local type=$1
    shift
    echo "[$type] $@"
}

function clean() {
    local BINARY="vengine"
    local BUILD_DIR=("build/CMakeFiles" "build/shaders")
    local DOC_DIRS=("documentation/.doxygen/html" "documentation/.doxygen/latex")

    clean_directory() {
        local dir="$1"
        if [ -d "$dir" ]; then
            rm -rf "$dir"/*
            log "INFO" "$dir directory has been cleaned."
        else
            log "WARNING" "$dir directory does not exist."
        fi
    }

    for dir in "${BUILD_DIR[@]}"; do
        clean_directory "$dir"
    done
    for dir in "${DOC_DIRS[@]}"; do
        clean_directory "$dir"
    done

    if [ -f "$BINARY" ]; then
        rm "$BINARY"
        log "INFO" "$BINARY binary has been removed."
    else
        log "WARNING" "$BINARY binary does not exist."
    fi
}

case $1 in
    build)
        "${CMAKE_CMD[@]}" && cmake --build build
        ;;
    clean)
        clean
        ;;
    format)
        "${CMAKE_CMD[@]}" -DUSE_CLANG_TIDY=ON && cmake --build build --target clangformat
        ;;
    doc)
        "${CMAKE_CMD[@]}" -DBUILD_DOC=ON && cmake --build build --target doxygen
        ;;
    *)
        log "ERROR" "Invalid command. Usage: $0 build | clean | format | doc"
        exit 1
        ;;
esac
