#!/bin/sh


# Functions

usage() {
    echo "usage: $(basename $0) [-t] <device uuid>"
    echo "<device uuid>\tUUID of the device to run on."
    echo "options:"
    echo "-t           \tBuild type (Debug or Release). Default is Release."
    echo ""
    echo "Available devices list:"
    xcrun simctl list
    exit 1
}


# Script body

BUILD_TYPE="Release"

while getopts "t:" opt
do
    case $opt in
    t)
        BUILD_TYPE="$OPTARG"
        ;;
    esac
done
shift $((OPTIND-1))

if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]] ; then
    usage
fi

DEVICE_UUID=$1

if [[ -z "$DEVICE_UUID" ]] ; then
    usage
fi

xcrun simctl boot $DEVICE_UUID
xcrun simctl install $DEVICE_UUID ../build/$BUILD_TYPE-iphonesimulator/Stronghold.app
xcrun simctl launch $DEVICE_UUID com.sourcehold.Stronghold
