#!/bin/bash

BINARIES=("vengine" "vengine-tests")
DIRS=("cmake-build-debug" "cmake-build-release" "cmake-build-tests" "cmake-build-doc" "cmake-build-format" "documentation/.doxygen/html" "documentation/.doxygen/latex")
EXCLUDED_DIRS=("cmake-build-debug/third-party" "cmake-build-release/third-party" "cmmake-build-doc/third-party" "cmake-build-format/third-party")

function log() {
    local type=$1
    shift
    echo "[$type] $@"
}

function clean_directory() {
    local dir="$1"
    if [ -d "$dir" ]; then
      for excluded in "${EXCLUDED_DIRS[@]}"; do
          if [[ "$dir" == *"$excluded"* ]]; then
              log "INFO" "Skipping directory $dir as it is excluded."
          else
              find "$dir" -mindepth 1 -not -path "$dir/$excluded/*" -exec rm -rf {} +
              log "INFO" "$dir directory has been cleaned."
          fi
      done
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
