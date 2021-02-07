#!/bin/sh

PROJECT_ROOT=".."
CMAKE_WORKING_DIR="$PROJECT_ROOT"
BUILD_DIR="$PROJECT_ROOT/build"

echo "Updating subrepositories..."
git submodule init
git submodule update

echo "Building..."
cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR"
cd "$BUILD_DIR"
make clean
make
