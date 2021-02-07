#!/bin/sh

CMAKE_WORKING_DIR="../"
BUILD_DIR="build"

echo "Updating subrepositories..."
git submodule init
git submodule update

echo "Building..."
cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR"
cd "$BUILD_DIR"
make clean
make
