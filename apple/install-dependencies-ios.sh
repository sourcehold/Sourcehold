#!/bin/sh


# Global variables

SCRIPT_PATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"


# Functions

usage() {
    echo "usage: $(basename $0) [-cds] <libraries dir>"
    echo "-c        \tClean build."
    echo "-d        \tDeployment target. Default value is '11.0'."
    echo "-s        \tBuild for Simulator. This option allows to skip compilation\n"
         "          \tfor some architectures when building dependencies to speed up build."
    exit 1
}

clean() {
    echo "Removing iOS dependecies..."
    local FF_MPEG_DIR_PATH="$1"
    local SDL2_DIR_PATH="$2"
    
    [ -d "$FF_MPEG_DIR_PATH" ] && rm -rf "$FF_MPEG_DIR_PATH"
    [ -d "$SDL2_DIR_PATH" ] && rm -rf "$SDL2_DIR_PATH"
}


# Script body

CLEAN=0
ARCHS="arm64"
DEPLOYMENT_TARGET=11.0
SDL2_INSTALL_OPTIONS=

while getopts "cd:s" opt
do
    case $opt in
    c)
        CLEAN=1
        ;;
    d)
        DEPLOYMENT_TARGET="$OPTARG"
        ;;
    s)
        ARCHS="x86_64"
        SDL2_INSTALL_OPTIONS="-s"
        ;;
    esac
done
shift $((OPTIND-1))

LIBS_DIR_PATH="$1"

if [[ -z "$LIBS_DIR_PATH" ]] ; then
    usage
fi

FF_MPEG_DIR_PATH="$LIBS_DIR_PATH/ffmpeg-ios"
SDL2_DIR_PATH="$LIBS_DIR_PATH/sdl2-ios"

if [[ $CLEAN -eq 1 ]] ; then
    clean "$FF_MPEG_DIR_PATH" "$SDL2_DIR_PATH"
fi

if [ ! -d "$FF_MPEG_DIR_PATH" ] ; then
    echo "Installing FFmpeg for iOS..."
    "$SCRIPT_PATH/install-ffmpeg-ios.sh" -r "$FF_MPEG_DIR_PATH" -d "$DEPLOYMENT_TARGET" -a "$ARCHS"
else
    echo "FFmpeg for iOS is already installed..."
fi

if [ ! -d "$SDL2_DIR_PATH" ] ; then
    echo "Installing SDL2 for iOS..."
    "$SCRIPT_PATH/install-sdl2-ios.sh" -r "$SDL2_DIR_PATH" -d "$DEPLOYMENT_TARGET" $SDL2_INSTALL_OPTIONS
else
    echo "SDL2 for iOS is already installed..."
fi
