#!/bin/bash

# Only work for LINUX Ubuntu system

case $1 in
    build)
        sudo apt update -y && sudo apt install -y libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev libglm-dev libglfw3-dev glslang-tools
        ;;
    doc)
        sudo apt update -y && sudo apt install -y libgl1-mesa-dev qt6-base-dev texlive-latex-base texlive-latex-recommended texlive-latex-extra graphviz
        ;;
    doxygen)
        sudo apt update -y
        wget https://www.doxygen.nl/files/doxygen-1.11.0.src.tar.gz && tar xf doxygen-1.11.0.src.tar.gz
        mkdir doxygen-1.11.0/build && cd doxygen-1.11.0/build
        cmake -G "Unix Makefiles" .. -Dbuild_wizard=YES && make && sudo make install
        ;;
    *)
        echo "Usage $0 build | doc | doxygen"
        exit 1
        ;;
esac
