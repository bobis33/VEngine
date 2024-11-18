#!/bin/bash

BINARIES=("vengine" "vengine-tests")
DIRS=("build" "documentation/.doxygen/html" "documentation/.doxygen/latex")
EXCLUDED_DIR="build/third-party"

function log() {
    local type=$1
    shift
    echo "[$type] $@"
}

function clean_directory() {
    local dir="$1"
    if [ -d "$dir" ]; then
        if [[ "$dir" == *"$EXCLUDED_DIR"* ]]; then
            log "INFO" "Skipping directory $dir as it is excluded."
        else
            find "$dir" -mindepth 1 -not -path "$dir/$EXCLUDED_DIR/*" -exec rm -rf {} +
            log "INFO" "$dir directory has been cleaned."
        fi
    else
        log "WARNING" "$dir directory does not exist."
    fi
}


for dir in "${DIRS[@]}"; do
    clean_directory "$dir"
done

for bin in "${BINARIES[@]}"; do
    if [ -f "$bin" ]; then
        rm "$bin"
        log "INFO" "$bin binary has been removed."
    else
        log "WARNING" "$bin binary does not exist."
    fi
done

