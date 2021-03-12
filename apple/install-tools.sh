#!/bin/sh
       

if [ ! `which brew` ] ; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" || exit 1
fi

if [ ! `which cmake` ] ; then
    echo "Installing cmake..."
    brew install cmake || exit 1
fi
