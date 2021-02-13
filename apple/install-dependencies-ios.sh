#!/bin/sh

CLEAN=0
LIBS_DIR=
FF_MPEG_DIR=
SDL2_DIR=


usage() {
    echo "usage: $(basename $0) [-c] <libraries dir>"
    echo "-c        \tClean build."
    exit 1
}

clean() {
    echo "Removing iOS dependecies..."
    [ -d "$FF_MPEG_DIR" ] && rm -rf "$FF_MPEG_DIR"
    [ -d "$SDL2_DIR" ] && rm -rf "$SDL2_DIR"
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

[ -z "$1" ] && usage

LIBS_DIR="$1"
FF_MPEG_DIR="$LIBS_DIR/ffmpeg-ios"
SDL2_DIR="$LIBS_DIR/sdl2-ios"

[[ $CLEAN -eq 1 ]] && clean

./install-ffmpeg-ios.sh -c -l -r "$FF_MPEG_DIR"
