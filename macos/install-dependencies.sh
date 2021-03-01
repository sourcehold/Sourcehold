#!/bin/sh

echo "Installing SDL2..."
brew install sdl2

echo "Installing FFmpeg..."
brew install ffmpeg

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
