#!/bin/sh

SRC_ROOT=".."
CMAKE_WORKING_DIR="$SRC_ROOT/"
BUILD_DIR="$SRC_ROOT/build"

echo "Updating subrepositories..."
git submodule init
git submodule update

echo "Building..."
cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR"
cd "$BUILD_DIR"
make clean
make
