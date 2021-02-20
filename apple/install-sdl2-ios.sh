#!/bin/sh


# Functions

usage() {
    echo "usage: $(basename $0) [-drv]"
    echo "options:"
    echo "-r        \tPath to result directory. Default value is '.'."
    echo "-v        \SDL2 lib version to compile. Default value is '4.3.1'"
    exit 1
}

fetch_source() {
    local SOURCE="$1"
    local RESULT_DIR="$2"
    local SRC_DIR="$RESULT_DIR/$SOURCE"
    
    if [ ! -d "$SRC_DIR" ] ; then
        echo "SDL2 source not found. Fetching $SOURCE..."
        local ARCHIVE_PATH="$RESULT_DIR/$SOURCE.zip"
        
        curl -L -o "$ARCHIVE_PATH" "https://www.libsdl.org/release/$SOURCE.zip"
        unzip -q "$ARCHIVE_PATH" -d "$RESULT_DIR"
        rm "$ARCHIVE_PATH"
    else
        echo "SDL2 library source directory already exists."
    fi
}

create_static_library() {
    local SRC_DIR="$1"
    local RESULT_DIR="$2"
    local PROJECT_PATH="$SRC_DIR/Xcode/SDL"
    local XCODE_PROJECT="SDL.xcodeproj"
    local XCODE_TARGET="Static Library-iOS"
    local LIB_NAME="libSDL2.a"
    local BUILD_DIR="$RESULT_DIR/build"
    local INCLUDE_DIR="$RESULT_DIR/include"
    local LIB_DIR="$RESULT_DIR/lib"

    pushd "$PROJECT_PATH" > /dev/null
    
    echo "Building the iOS iPhone static library ..."

    xcrun xcodebuild -configuration "Release" \
        -project "$XCODE_PROJECT" \
        -target "$XCODE_TARGET" \
        -sdk "iphoneos" \
        build \
        ONLY_ACTIVE_ARCH=NO \
        RUN_CLANG_STATIC_ANALYZER=NO \
        BUILD_DIR="$BUILD_DIR" \
        SYMROOT="$BUILD_DIR" \
        OBJROOT="$BUILD_DIR/obj" \
        BUILD_ROOT="$BUILD_DIR" \
        TARGET_BUILD_DIR="$BUILD_DIR/iphoneos"

    echo "Building the iOS Simulator static library ..."

    xcrun xcodebuild -configuration "Release" \
        -project "$XCODE_PROJECT" \
        -target "$XCODE_TARGET" \
        -sdk "iphonesimulator" \
        -arch i386 -arch x86_64 \
        build \
        ONLY_ACTIVE_ARCH=NO \
        RUN_CLANG_STATIC_ANALYZER=NO \
        BUILD_DIR="$BUILD_DIR" \
        SYMROOT="$BUILD_DIR" \
        OBJROOT="$BUILD_DIR/obj" \
        BUILD_ROOT="$BUILD_DIR" \
        TARGET_BUILD_DIR="$BUILD_DIR/iphonesimulator"

    echo "Creating fat library ..."

    mkdir "$LIB_DIR"
    xcrun -sdk iphoneos lipo -create \
        -output "$LIB_DIR/$LIB_NAME" \
        "$BUILD_DIR/iphoneos/$LIB_NAME" \
        "$BUILD_DIR/iphonesimulator/$LIB_NAME"

    echo "The fat static library '$LIB_NAME' is ready."
    
    popd > /dev/null
    
    echo "Copying headers..."
    mkdir "$INCLUDE_DIR"
    cp `find "$SRC_DIR/include" -name *.h` "$INCLUDE_DIR"
}

cleanup() {
    echo "Cleaning up..."
    local RESULT_DIR="$1"
    
    rm -R "$RESULT_DIR/build"
}


#Script body

SDL2_VERSION="2.0.14"
RESULT_DIR="."

while getopts "r:v:" opt
do
    case $opt in
    r)
        RESULT_DIR="$OPTARG"
        ;;
    v)
        SDL2_VERSION="$OPTARG"
        ;;
    ?)
        usage
        ;;
    esac
done

# RESULT_DIR should be converted to absolute path to make 'create_static_library' function work properly
mkdir -p "$RESULT_DIR"
RESULT_DIR=`cd "$RESULT_DIR"; pwd`

SOURCE="SDL2-$SDL2_VERSION"
SRC_DIR="$RESULT_DIR/$SOURCE"

fetch_source "$SOURCE" "$RESULT_DIR"
create_static_library "$SRC_DIR" "$RESULT_DIR"
cleanup "$RESULT_DIR"
