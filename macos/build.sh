#!/bin/sh

which -s brew
if [[ $? != 0 ]] ; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
    echo "Updating Homebrew..."
    brew update
fi

echo "Installing SDL2..."
brew install sdl2

echo "Installing FFmpeg..."
brew install ffmpeg

echo "Building..."
cd ../
cmake . -B build
cd build
make clean
make

echo "Setting up environment..."
SAVES_DIR="saves"
if [ -d "$SAVES_DIR" ]; then
     echo "Creating 'saves' directory..."
     mkdir "$SAVES_DIR"
fi
