# Sourcehold [![Build Status](https://travis-ci.org/sourcehold/Sourcehold.svg?branch=master)](https://travis-ci.org/sourcehold/Sourcehold) [![Discord](https://img.shields.io/discord/566293677329219595.svg?color=7389D8&label=%20&logo=discord&logoColor=ffffff)](https://discord.gg/dzdBuNy)

Open source engine implementation of Stronghold by Firefly Studios. This project is currently in an early stage of development. Once completed, it should be able to replace all editions of classic Stronghold, including the HD remaster.

---

![Imgur](https://i.imgur.com/rkk3kAt.png)

## About
Stronghold is a castle sim created by Firefly Studios in 2001. It was released for Windows and supports MacOS since
its HD remaster.
The purpose of this project is to make Stronghold playable on platforms it didn't support originally. It is not intended to be a 100% accurate source port, so there will be differences.

## Building
After cloning the repository, make sure to run `git submodule init` and `git submodule update` to fetch [cxxopts](https://github.com/jarro2783/cxxopts). 

Sourcehold uses the following libraries:
*   SDL2
*   OpenAL
*   FFmpeg (avcodec, avutil, avformat, swscale)
*   zlib's blast for PKWARE decompression
*   EnTT

Run `cmake` in your build directory.

### Windows

You will have to build the dependencies from source and the most convenient way is to use [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160).  
This may take a long time, depending on your machine.  
Alternatively you may install everything manually (not recommended).

#### Quickstart

Installing vcpkg:
```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg integrate install
```
Installing the dependencies:
```bash
vcpkg --triplet x64-windows install sdl2 ffmpeg openal-soft
```
Then continue building Sourcehold with CMake:
```bash
cmake -H. -B build -DCMAKE_TOOLCHAIN_FILE=%VCPKG_FOLDER%\scripts\buildsystems\vcpkg.cmake
cmake --build build
```
### Mac OS
Simply run `set-up-n-build.sh` from `macos` directory in repository root. This script will install Homebrew and all necessary dependencies, build project and create all necessary directories to run Sourcehold.

Also you may build and run Sourcehold using Xcode. Project file is located inside `macos` directory in repository root.

## Running
Make sure you point Sourcehold to where your game data is located, which you can do using `--path=/your/path` or copy
the files to a directory called `data` in the main folder of Sourcehold. The directory should look like this:

```
data
├── binks/
├── fx/
├── gfx/
├── gfx8/
├── gm/
├── help/
├── maps/
├── stronghold.mlb
├── sh.tex
├── delete.ani
├── hand.ani
├── jester.ani
└── sword.ani
```

## Configuration
This is either done by reading an existing Stronghold config file, found in your documents folder
under `Stronghold/stronghold.cfg`, via command line or both. Options from the command line will
overwrite the ones from the config. Available options are listed with the `--help` argument. You can also add your command line options to a file called `settings.ini`, in the form `option=value`.
