#!/bin/bash

CMAKE_CMD=(cmake -S . -Bbuild -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release)

function log() {
    local type=$1
    shift
    echo "[$type] $@"
}

function clean() {
    local BINARY="vengine"
    local DIRS=("build" "documentation/.doxygen/html" "documentation/.doxygen/latex")
    local EXCLUDED_DIR="third-party"
    function clean_directory() {
        local dir="$1"
        if [ -d "$dir" ]; then
            if [ "$dir" == *"$EXCLUDED_DIR"* ]; then
                log "INFO" "Skipping directory $dir as it is excluded."
            else
                find "$dir" -mindepth 1 ! -path "$dir/$EXCLUDED_DIR/*" -delete
                log "INFO" "$dir directory has been cleaned."
            fi
        else
            log "WARNING" "$dir directory does not exist."
        fi
    }

    for dir in "${DIRS[@]}"; do
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
