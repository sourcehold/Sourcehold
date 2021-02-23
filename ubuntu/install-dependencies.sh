#!/bin/sh

sudo apt install libsdl2-dev libopenal-dev libavcodec-dev libavformat-dev libavutil-dev libavfilter-dev libswscale-dev libpostproc-dev libswresample-dev

mkdir /deps
cd /deps

git clone --depth 1 https://github.com/skypjack/entt
cd entt
mkdir build
cd build
cmake ..
make
sudo make install
