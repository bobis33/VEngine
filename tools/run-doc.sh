#!/bin/bash

function nodeServer() {
    npx http-server documentation/.doxygen/html/ -p 8080
}

function phpServer() {
    php -S localhost:8080 -t documentation/.doxygen/html/
}

function pythonServer() {
    python3 -m http.server 8080 --directory documentation/.doxygen/html/
}

case $1 in
    node)
        nodeServer
        ;;
    php)
        phpServer
        ;;
    py)
        pythonServer
        ;;
    *)
        echo "Usage $0 node | php | python"
        exit 1
        ;;
esac
