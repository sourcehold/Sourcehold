# Sourcehold

## Notice
This project is frozen until we know what the new [Definitive Edition of Stronghold](https://store.steampowered.com/app/2140020/Stronghold_Definitive_Edition/) from Firefly will be able to do!

We have a Discord server!
[![Discord](https://img.shields.io/discord/1259903348077756527.svg?color=7389D8&label=%20&logo=discord&logoColor=ffffff)](https://discord.gg/WdjYD39gJE)

Looking for a Stronghold (Crusader) community? Join the UCP Discord
[![Discord](https://img.shields.io/discord/426318193603117057.svg?color=7389D8&label=%20&logo=discord&logoColor=ffffff)](https://discord.gg/anzCpnTGxY)

## Project description
[![Build Status](https://github.com/sourcehold/Sourcehold/actions/workflows/sourcehold.yml/badge.svg?event=push)](https://github.com/sourcehold/Sourcehold/actions/workflows/sourcehold.yml)

Open source engine implementation of Stronghold by Firefly Studios. This project is currently in an early stage of development. Once completed, it should be able to replace all editions of classic Stronghold, including the HD remaster.

---

![Imgur](https://i.imgur.com/rkk3kAt.png)

## About
Stronghold is a castle sim created by Firefly Studios in 2001. It was released for Windows and supports MacOS since
its HD remaster.
The purpose of this project is to make Stronghold playable on platforms it didn't support originally. It is not intended to be a 100% accurate source port, so there will be differences.

## Building

Sourcehold uses the following libraries:

* SDL2
* OpenAL
* FFmpeg (avcodec, avutil, avformat, swscale)
* zlib's blast for PKWARE decompression
* [EnTT](https://github.com/skypjack/entt) as submodule
* [filesystem](https://github.com/gulrak/filesystem) as submodule
* [cxxopts](https://github.com/jarro2783/cxxopts) as submodule

After cloning the repository, make sure to run `git submodule init` and `git submodule update` to fetch [EnTT](https://github.com/skypjack/entt), [filesystem](https://github.com/gulrak/filesystem) and [cxxopts](https://github.com/jarro2783/cxxopts).

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
Simply run `build.sh macos` from `apple` directory in the repository root. This script will install Homebrew and all necessary dependencies and build project. To build debug configuration pass `-t Debug` option to `build.sh`.

**Note:** You should install Xcode before trying to build project using `build.sh` script. Also do not forget to pass `-c` option to `build.sh` if you build Mac OS version of the Sourcehold after iOS version.

### iOS
Currently you can not build for iOS devices. Only iOS simulator is supported. To build Sourcehold for the iOS simulator run `build.sh ios-simulator` from `apple` directory in the repository root. Script will install Homebrew and all necessary tools. Also it will compile and install SDL2 and FFmpeg libraries to `thirdparty/ios` directory in the repository root. To build debug configuration pass `-t Debug` option to `build.sh`. 

After building project with `build.sh` script you may also peform subsequent builds using Xcode. To do that simply open `Stronghold.xcodeproj` from cmake build directory.

**Note:** You should install Xcode before trying to build project using `build.sh` script. Also do not forget to pass `-c` option to `build.sh` if you build iOS version of the Sourcehold after Mac OS version.

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

### iOS
You may run iOS version on the simulator using `run-on-ios-simulator.sh` script. You should pass simulator UUID to this script. Run it without parameters to get list of available simulators and their UUIDs. By default `run-on-ios-simulator.sh` tries to run release configuration. Pass `-t Debug` option to alter this behaviour.

You also may just drag'n'drop application bundle (Stronghold.app) to the simulator and run it manually.

**Note:** Currently `data` folder is embedded into application bundle resources, thus you should put it in the main folder of Sourcehold before you build the project.

## Configuration
This is either done by reading an existing Stronghold config file, found in your documents folder
under `Stronghold/stronghold.cfg`, via command line or both. Options from the command line will
overwrite the ones from the config. Available options are listed with the `--help` argument. You can also add your command line options to a file called `settings.ini`, in the form `option=value`.

### iOS
On iOS configuration is not currently supported.
