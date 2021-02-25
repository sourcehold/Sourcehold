#!/bin/sh


# Functions

usage() {
    echo "usage: $(basename $0) [-drsv]"
    echo "options:"
    echo "-d        \tiOS deployment target. Default is '10.0'"
    echo "-r        \tPath to result directory. Default value is '.'."
    echo "-s        \tBuild for Simulator. This option allows to skip unnecessary architectures\n"
         "          \tcompilation to speed up build."
    echo "-v        \SDL2 lib version to compile. Default value is '4.3.1'"
    exit 1
}

fetch_source() {
    local SOURCE="$1"
    local RESULT_DIR_PATH="$2"
    local SRC_DIR_PATH="$RESULT_DIR_PATH/$SOURCE"
    
    if [ ! -d "$SRC_DIR_PATH" ] ; then
        echo "SDL2 source not found. Fetching $SOURCE..."
        local ARCHIVE_PATH="$RESULT_DIR_PATH/$SOURCE.zip"
        
        curl -L -o "$ARCHIVE_PATH" "https://www.libsdl.org/release/$SOURCE.zip"
        unzip -q "$ARCHIVE_PATH" -d "$RESULT_DIR_PATH"
        rm "$ARCHIVE_PATH"
    else
        echo "SDL2 library source directory already exists."
    fi
}

create_static_library() {
    local SRC_DIR_PATH="$1"
    local RESULT_DIR_PATH="$2"
    local DEPLOYMENT_TARGET="$3"
    local ONLY_SIMULATOR_BUILD="$4"
    local PROJECT_PATH="$SRC_DIR_PATH/Xcode/SDL"
    local XCODE_PROJECT="SDL.xcodeproj"
    local XCODE_TARGET="Static Library-iOS"
    local CONFIGURATION="Release"
    local LIB_NAME="libSDL2.a"
    local BUILD_DIR_PATH="$RESULT_DIR_PATH/build"
    local INCLUDE_DIR_PATH="$RESULT_DIR_PATH/include"
    local LIB_DIR_PATH="$RESULT_DIR_PATH/lib"

    pushd "$PROJECT_PATH" > /dev/null

    if [[ $ONLY_SIMULATOR_BUILD -ne 1 ]] ; then
        echo "Building the iOS iPhone static library ..."
        
        xcrun xcodebuild -configuration "$CONFIGURATION" \
            -project "$XCODE_PROJECT" \
            -target "$XCODE_TARGET" \
            -sdk "iphoneos" \
            build \
            IPHONEOS_DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET \
            ONLY_ACTIVE_ARCH=NO \
            RUN_CLANG_STATIC_ANALYZER=NO \
            BUILD_DIR_PATH="$BUILD_DIR_PATH" \
            SYMROOT="$BUILD_DIR_PATH" \
            OBJROOT="$BUILD_DIR_PATH/obj" \
            BUILD_ROOT="$BUILD_DIR_PATH" \
            TARGET_BUILD_DIR_PATH="$BUILD_DIR_PATH/iphoneos"
    fi

    echo "Building the iOS Simulator static library ..."

    xcrun xcodebuild -configuration "$CONFIGURATION" \
        -project "$XCODE_PROJECT" \
        -target "$XCODE_TARGET" \
        -sdk "iphonesimulator" \
        -arch i386 -arch x86_64 \
        build \
        IPHONEOS_DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET \
        ONLY_ACTIVE_ARCH=NO \
        RUN_CLANG_STATIC_ANALYZER=NO \
        BUILD_DIR_PATH="$BUILD_DIR_PATH" \
        SYMROOT="$BUILD_DIR_PATH" \
        OBJROOT="$BUILD_DIR_PATH/obj" \
        BUILD_ROOT="$BUILD_DIR_PATH" \
        TARGET_BUILD_DIR_PATH="$BUILD_DIR_PATH/iphonesimulator"

    mkdir "$LIB_DIR_PATH"
    if [[ $ONLY_SIMULATOR_BUILD -eq 1 ]] ; then
        echo "Copying fat library ..."
        cp "$BUILD_DIR_PATH/$CONFIGURATION-iphonesimulator/$LIB_NAME" "$LIB_DIR_PATH/$LIB_NAME"
    else
        echo "Creating fat library ..."
        xcrun -sdk iphoneos lipo -create \
            -output "$LIB_DIR_PATH/$LIB_NAME" \
            "$BUILD_DIR_PATH/$CONFIGURATION-iphoneos/$LIB_NAME" \
            "$BUILD_DIR_PATH/$CONFIGURATION-iphonesimulator/$LIB_NAME"
    fi

    echo "The fat static library '$LIB_NAME' is ready."
    
    popd > /dev/null
    
    echo "Copying headers..."
    mkdir "$INCLUDE_DIR_PATH"
    cp `find "$SRC_DIR_PATH/include" -name *.h` "$INCLUDE_DIR_PATH"
}

cleanup() {
    echo "Cleaning up..."
    local RESULT_DIR_PATH="$1"
    
    rm -R "$RESULT_DIR_PATH/build"
}


#Script body

SDL2_VERSION="2.0.14"
RESULT_DIR_PATH="."
DEPLOYMENT_TARGET=10.0
ONLY_SIMULATOR_BUILD=0

while getopts "d:r:sv:" opt
do
    case $opt in
    d)
        DEPLOYMENT_TARGET="$OPTARG"
        ;;
    r)
        RESULT_DIR_PATH="$OPTARG"
        ;;
    s)
        ONLY_SIMULATOR_BUILD=1
        ;;
    v)
        SDL2_VERSION="$OPTARG"
        ;;
    ?)
        usage
        ;;
    esac
done

echo "DEPLO = $DEPLOYMENT_TARGET, ONLY SIM = $ONLY_SIMULATOR_BUILD"

# RESULT_DIR_PATH should be converted to absolute path to make 'create_static_library' function work properly
mkdir -p "$RESULT_DIR_PATH"
RESULT_DIR_PATH=`cd "$RESULT_DIR_PATH"; pwd`

SOURCE="SDL2-$SDL2_VERSION"
SRC_DIR_PATH="$RESULT_DIR_PATH/$SOURCE"

fetch_source "$SOURCE" "$RESULT_DIR_PATH"
create_static_library "$SRC_DIR_PATH" "$RESULT_DIR_PATH" "$DEPLOYMENT_TARGET" "$ONLY_SIMULATOR_BUILD"
cleanup "$RESULT_DIR_PATH"
