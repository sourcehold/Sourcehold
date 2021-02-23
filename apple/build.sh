#!/bin/sh


# Global variables

PROJECT_ROOT=".."
CMAKE_WORKING_DIR="$PROJECT_ROOT"
BUILD_DIR="$PROJECT_ROOT/build"
IOS_LIBS_DIR="$PROJECT_ROOT/thirdparty/ios"


# Functions

usage() {
    echo "usage: $(basename $0) [-c] <platform>"
    echo "options:"
    echo "-c        \tClean build."
    echo "<platform>\t'ios' or 'macos'"
    exit 1
}

clean() {
    echo "Cleaning up..."
    [ -d "$BUILD_DIR" ] && rm -rf "$BUILD_DIR"
}

install_dependecies() {
    echo "Installing dependencies..."
    local CLEAN="$1"
    local PLATFORM="$2"
    
    if [[ "$PLATFORM" == "ios" ]] ; then
        local OPTIONS=
        
        if [[ $CLEAN -eq 1 ]] ; then
            OPTIONS="-c"
        fi

        mkdir -p "$IOS_LIBS_DIR"    
        ./install-dependencies-ios.sh $OPTIONS "$IOS_LIBS_DIR"
    else
        ./install-dependencies-macos.sh
    fi
}

update_submodules() {
    echo "Updating submodules..."
    git submodule init
    git submodule update
}

build() {
    echo "Building..."
    local CLEAN="$1"
    local CLEAN_OPTION=
    
    if [[ $CLEAN -eq 1 ]] ; then
        CLEAN_OPTION="--clean-first"
    fi
    
    if [[ "$PLATFORM" == "ios" ]] ; then
        local SYSROOT=`xcode-select -p`/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk

        cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR" -GXcode \
            -DCMAKE_SYSTEM_NAME=iOS \
            -DCMAKE_OSX_DEPLOYMENT_TARGET=10.0 \
            -DCMAKE_APPLE_ARCH_SYSROOTS="$SYSROOT;$SYSROOT" \
            "-DCMAKE_OSX_ARCHITECTURES=i386;x86_64"
        cmake --build "$BUILD_DIR" --config Release $CLEAN_OPTION -- -sdk iphonesimulator
    else
        cmake "$CMAKE_WORKING_DIR" -B "$BUILD_DIR"
        cmake --build "$BUILD_DIR" $CLEAN_OPTION
    fi
}


# Script body

CLEAN=0

while getopts "c" opt
do
    case $opt in
    c)
        CLEAN=1
        ;;
    esac
done
shift $((OPTIND-1))

PLATFORM="$1"

if [[ -z "$PLATFORM" ]] ; then
    usage
fi

if [[ "$PLATFORM" != "ios" && "$PLATFORM" != "macos" ]] ; then
    usage
fi

if [[ $CLEAN -eq 1 ]] ; then
    clean
fi

./install-tools.sh
install_dependecies $CLEAN "$PLATFORM"
update_submodules
build $CLEAN "$PLATFORM"
