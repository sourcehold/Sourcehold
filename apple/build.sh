#!/bin/sh

PROJECT_ROOT=".."
CMAKE_WORKING_DIR="$PROJECT_ROOT"
BUILD_DIR="$PROJECT_ROOT/build"
LIBS_DIR="$PROJECT_ROOT/thirdparty"
CLEAN=0


usage() {
    echo "usage: $(basename $0) [-c] <platform>"
    echo "options:"
    echo "-c        \tClean build."
    echo "<platform>\t'ios' or 'macos'"
    exit 1
}

clean() {
    echo "Cleaning up..."
    [ -d $BUILD_DIR ] && rm -rf "$BUILD_DIR"
}

installDependecies() {
    echo "Installing dependencies..."
    mkdir -p "$LIBS_DIR"
    
    local PLATFORM="$1"
    
    if [[ "$PLATFORM" == "ios" ]] ; then
        local OPTIONS=
        
        if [[ $CLEAN -eq 1 ]] ; then
            OPTIONS="-c"
        fi
        
        ./install-dependencies-ios.sh "$OPTIONS" "$LIBS_DIR"
    else
        ./install-dependencies-macos.sh
    fi
}

updateSubmodules() {
    echo "Updating submodules..."
    git submodule init
    git submodule update
}

build() {
    echo "Building..."
    cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR"
    cd "$BUILD_DIR"
    make clean
    make
}


while getopts c opt
do
    case $opt in
    c)
        CLEAN=1
        ;;
    esac
done
shift $((OPTIND-1))


PLATFORM="$1"
[ -z $PLATFORM ] && usage
[[ "$PLATFORM" != "ios" && "$PLATFORM" != "macos" ]] && usage


[[ $CLEAN -eq 1 ]] && clean "$PLATFORM"

installDependecies "$PLATFORM"
updateSubmodules
build
