#!/bin/sh
        
if [ ! `which brew` ] ; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" || exit 1
fi

if [ ! `which cmake` ] ; then
    echo "Installing cmake..."
    brew install cmake || exit 1
fi

if [ ! `which yasm` ] ; then
    echo "Installing yasm..."
    brew install yasm || exit 1
fi

if [ ! `which gas-preprocessor.pl` ] ; then
    echo "Installing gas-preprocessor.pl..."
    (curl -L https://github.com/libav/gas-preprocessor/raw/master/gas-preprocessor.pl \
            -o /usr/local/bin/gas-preprocessor.pl \
            && chmod +x /usr/local/bin/gas-preprocessor.pl) \
            || exit 1
fi
