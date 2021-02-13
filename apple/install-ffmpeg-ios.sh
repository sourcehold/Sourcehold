#!/bin/sh

COMPILE=0
LIPO=0

FF_VERSION="4.3.1"
SOURCE=
# RESULT_DIR should be converted to absolute path to make 'compile' function work properly
RESULT_DIR="."
SRC_DIR=
BUILD_DIR=
INCLUDE_DIR=
LIB_DIR=
FAT_LIB_NAME="fat"

ARCHS="arm64 armv7 x86_64 i386"
DEPLOYMENT_TARGET="9.0"
CONFIGURE_FLAGS="--enable-cross-compile --disable-debug --disable-programs \
                 --disable-doc --enable-pic"


usage() {
    echo "usage: $(basename $0) [-acdlrv]"
    echo "options:"
    echo "-a        \tArchitectures to build. You may pass multiple architectures. Default value is 'arm64 armv7 i386 x86_64'"
    echo "-c        \tIf this option is omitted, then compilation is skipped."
    echo "-d        \tiOS deployment target. Default is '9.0'"
    echo "-l        \tWhether 'lipo' or not result binaries."
    echo "-r        \tPath to result directory. Default value is '.'."
    echo "-v        \tFFmpeg lib version to compile. Default value is '4.3.1'"
    exit 1
}

compile() {
    if [ ! -d "$SRC_DIR" ]
    then
        echo 'FFmpeg source not found. Trying to download...'
        curl http://www.ffmpeg.org/releases/$SOURCE.tar.bz2 | tar xj --directory "$RESULT_DIR" \
            || exit 1
    fi

    for ARCH in $ARCHS
    do
        echo "Building $ARCH..."
        mkdir -p "$BUILD_DIR/$ARCH"
        pushd "$BUILD_DIR/$ARCH" > /dev/null

        local CFLAGS="-arch $ARCH"
        local PLATFORM=
        local EXPORT=
        
        if [ "$ARCH" = "i386" -o "$ARCH" = "x86_64" ] ; then
            PLATFORM="iPhoneSimulator"
            CFLAGS="$CFLAGS -mios-simulator-version-min=$DEPLOYMENT_TARGET"
        else
            PLATFORM="iPhoneOS"
            CFLAGS="$CFLAGS -mios-version-min=$DEPLOYMENT_TARGET -fembed-bitcode"
            if [ "$ARCH" = "arm64" ] ; then
                EXPORT="GASPP_FIX_XCODE5=1"
            fi
        fi

        local XCRUN_SDK=`echo $PLATFORM | tr '[:upper:]' '[:lower:]'`
        local CC="xcrun -sdk $XCRUN_SDK clang"

        # force "configure" to use "gas-preprocessor.pl" (FFmpeg 3.3)
        if [ "$ARCH" = "arm64" ] ; then
            AS="gas-preprocessor.pl -arch aarch64 -- $CC"
        else
            AS="gas-preprocessor.pl -- $CC"
        fi

        CXXFLAGS="$CFLAGS"
        LDFLAGS="$CFLAGS"

        TMPDIR=${TMPDIR/%\/} "$SRC_DIR/configure" \
            --target-os=darwin \
            --arch=$ARCH \
            --cc="$CC" \
            --as="$AS" \
            $CONFIGURE_FLAGS \
            --extra-cflags="$CFLAGS" \
            --extra-ldflags="$LDFLAGS" \
            --prefix="$LIB_DIR/$ARCH" \
        || exit 1

        make -j3 install $EXPORT || exit 1
        popd > /dev/null
    done
}

createFatBinaries() {
    echo "Building fat binaries..."
    local FAT_LIB_DIR="$LIB_DIR/$FAT_LIB_NAME/lib"
    local FAT_INCLUDE_DIR="$LIB_DIR/$FAT_LIB_NAME/include"
    
    mkdir -p "$FAT_LIB_DIR"
    set - $ARCHS
    local CWD=`pwd`
    
    pushd "$LIB_DIR/$1/lib" > /dev/null
    for LIB in *.a
    do
        echo lipo -create `find $LIB_DIR -name $LIB` -output "$FAT_LIB_DIR/$LIB" 1>&2
        lipo -create `find $LIB_DIR -name $LIB` -output "$FAT_LIB_DIR/$LIB" || exit 1
    done

    popd > /dev/null
    cp -rf "$LIB_DIR/$1/include" "$FAT_INCLUDE_DIR"
}

while getopts "a:cd:lr:v:" opt
do
    case $opt in
    a)
        ARCH="$OPTARG"
        ;;
    c)
        COMPILE=1
        ;;
    d)
        DEPLOYMENT_TARGET="$OPTARG"
        ;;
    l)
        LIPO=1
        ;;
    r)
        RESULT_DIR="$OPTARG"
        ;;
    v)
        FF_VERSION="$OPTARG"
        ;;
    ?)
        usage
        ;;
    esac
done


[[ $COMPILE -eq 0 && $LIPO -eq 0 ]] && usage

mkdir -p "$RESULT_DIR"
#to get absolute path
RESULT_DIR=`cd "$RESULT_DIR"; pwd`

SOURCE="ffmpeg-$FF_VERSION"
SRC_DIR="$RESULT_DIR/$SOURCE"
BUILD_DIR="$RESULT_DIR/build"
INCLUDE_DIR="$RESULT_DIR/include"
LIB_DIR="$RESULT_DIR/lib"

if [[ $COMPILE -eq 1 ]]
then
    compile
fi

if [[ $LIPO -eq 1 ]]
then
    createFatBinaries
fi

echo Done
