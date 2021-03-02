#!/bin/sh


# Functions

usage() {
    echo "usage: $(basename $0) <device uuid>"
    echo "<device uuid>\tUUID of the device to run on."
    echo ""
    echo "Available devices list:"
    xcrun simctl list
    exit 1
}


# Script body

DEVICE_UUID=$1

if [[ -z "$DEVICE_UUID" ]] ; then
    usage
fi

BUILD_TYPE=Release

xcrun simctl boot $DEVICE_UUID
xcrun simctl install $DEVICE_UUID ../build/$BUILD_TYPE-iphonesimulator/Stronghold.app
xcrun simctl launch $DEVICE_UUID com.sourcehold.Stronghold
