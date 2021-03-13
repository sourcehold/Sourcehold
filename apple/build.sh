#!/bin/sh


# Global variables

SCRIPT_PATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT_PATH="$SCRIPT_PATH/.."
CMAKE_WORKING_DIR_PATH="$PROJECT_ROOT_PATH"
BUILD_DIR_PATH="$PROJECT_ROOT_PATH/build"
IOS_LIBS_DIR_PATH="$PROJECT_ROOT_PATH/thirdparty/ios"
IOS_DEPLOYMENT_TARGET=11.0


# Functions

usage() {
    echo "usage: $(basename $0) [-ct] <platform>"
    echo "options:"
    echo "-c        \tClean build."
    echo "-t        \tBuild type (Debug or Release). Default is Release."
    echo "<platform>\t'ios', 'ios-simulator' or 'macos'"
    exit 1
}

clean() {
    echo "Cleaning up..."
    [ -d "$BUILD_DIR_PATH" ] && rm -rf "$BUILD_DIR_PATH"
}

install_dependecies() {
    echo "Installing dependencies..."
    local CLEAN="$1"
    local PLATFORM="$2"
    
    if [[ "$PLATFORM" == "ios-simulator" || "$PLATFORM" == "ios" ]] ; then
        local OPTIONS="-d $IOS_DEPLOYMENT_TARGET"
        
        if [[ $CLEAN -eq 1 ]] ; then
            OPTIONS="$OPTIONS -c"
        fi
        
        if [[ "$PLATFORM" == "ios-simulator" ]] ; then
            OPTIONS="$OPTIONS -s"
        fi

        mkdir -p "$IOS_LIBS_DIR_PATH"
        "$SCRIPT_PATH/install-dependencies-ios.sh" $OPTIONS "$IOS_LIBS_DIR_PATH"
    else
        "$SCRIPT_PATH/install-dependencies-macos.sh"
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
    local BUILD_TYPE="$2"
    local PLATFORM="$3"
    local CLEAN_OPTION=
    
    if [[ $CLEAN -eq 1 ]] ; then
        CLEAN_OPTION="--clean-first"
    fi
    
    if [[ "$PLATFORM" == "ios-simulator" ]] ; then
        local SYSROOT=`xcode-select -p`/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk

        cmake "$CMAKE_WORKING_DIR_PATH" -B "$BUILD_DIR_PATH" -GXcode \
            -DCMAKE_SYSTEM_NAME=iOS \
            -DCMAKE_OSX_DEPLOYMENT_TARGET=$IOS_DEPLOYMENT_TARGET \
            -DCMAKE_APPLE_ARCH_SYSROOTS="$SYSROOT" \
            -DCMAKE_OSX_ARCHITECTURES="x86_64" \
            -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
        cmake --build "$BUILD_DIR_PATH" --config $BUILD_TYPE $CLEAN_OPTION -- -sdk iphonesimulator
    else
        cmake "$CMAKE_WORKING_DIR_PATH" -B "$BUILD_DIR_PATH"
        cmake --build "$BUILD_DIR_PATH" --config $BUILD_TYPE $CLEAN_OPTION
    fi
}


# Script body

CLEAN=0
BUILD_TYPE="Release"

while getopts "ct:" opt
do
    case $opt in
    c)
        CLEAN=1
        ;;
    t)
        BUILD_TYPE="$OPTARG"
        ;;
    esac
done
shift $((OPTIND-1))

if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]] ; then
    usage
fi

PLATFORM="$1"

if [[ -z "$PLATFORM" ]] ; then
    usage
fi

if [[ "$PLATFORM" == "ios" ]] ; then
    echo "Build for device is not currently supported."
    exit 1
fi

if [[ "$PLATFORM" != "ios" && "$PLATFORM" != "ios-simulator" && "$PLATFORM" != "macos" ]] ; then
    usage
fi

if [ ! `which xcodebuild` ] ; then
  echo "Build failed. Xcode is not installed.";
  exit 1
fi

if [[ $CLEAN -eq 1 ]] ; then
    clean
fi

"$SCRIPT_PATH/install-tools.sh"
install_dependecies $CLEAN "$PLATFORM"
update_submodules
build $CLEAN "$BUILD_TYPE" "$PLATFORM"
