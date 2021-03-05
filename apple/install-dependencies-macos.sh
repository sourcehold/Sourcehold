#!/bin/sh


# Functions

function is_brew_formula_installed {
    local BREW_FORMULA="$1"

    brew list "$BREW_FORMULA" &> /dev/null
    return $?
}


# Script body

if ! is_brew_formula_installed ffmpeg ; then
    echo "Installing FFmpeg for macOS..."
    brew install ffmpeg
else
    echo "FFmpeg for macOS is already installed..."
fi

if ! is_brew_formula_installed sdl2 ; then
    echo "Installing SDL2 for macOS..."
    brew install sdl2
else
    echo "SDL2 for macOS is already installed..."
fi

echo "Installing skypjacks EnTT..."
# brew install skypjack/entt/entt

mkdir /deps
cd /deps

git clone --depth 1 https://github.com/skypjack/entt
cd entt
mkdir build
cd build
cmake ..
make
sudo make install

