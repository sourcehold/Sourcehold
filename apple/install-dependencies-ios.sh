#!/bin/sh


# Global variables

SCRIPT_PATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"


# Functions

usage() {
    echo "usage: $(basename $0) [-c] <libraries dir>"
    echo "-c        \tClean build."
    exit 1
}

clean() {
    echo "Removing iOS dependecies..."
    local FF_MPEG_DIR="$1"
    local SDL2_DIR="$2"
    
    [ -d "$FF_MPEG_DIR" ] && rm -rf "$FF_MPEG_DIR"
    [ -d "$SDL2_DIR" ] && rm -rf "$SDL2_DIR"
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

LIBS_DIR="$1"

if [[ -z "$LIBS_DIR" ]] ; then
    usage
fi

FF_MPEG_DIR="$LIBS_DIR/ffmpeg-ios"
SDL2_DIR="$LIBS_DIR/sdl2-ios"

if [[ $CLEAN -eq 1 ]] ; then
    clean "$FF_MPEG_DIR" "$SDL2_DIR"
fi

if [ ! -d "$FF_MPEG_DIR" ] ; then
    echo "Installing FFmpeg for iOS..."
    "$SCRIPT_PATH/install-ffmpeg-ios.sh" -r "$FF_MPEG_DIR"
else
    echo "FFmpeg for iOS is already installed..."
fi

if [ ! -d "$SDL2_DIR" ] ; then
    echo "Installing SDL2 for iOS..."
    "$SCRIPT_PATH/install-sdl2-ios.sh" -r "$SDL2_DIR"
else
    echo "SDL2 for iOS is already installed..."
fi
