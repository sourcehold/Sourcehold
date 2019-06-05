Sourcehold [![Build Status](https://travis-ci.org/metalvoidzz/Sourcehold.svg?branch=master)](https://travis-ci.org/metalvoidzz/Sourcehold)
======

Open source engine implementation of Stronghold by Firefly Studios. This project is currently in an early stage of development. Once completed, it should be able to replace all editions of classic Stronghold, including the HD remaster.

---

![Imgur](https://i.imgur.com/rkk3kAt.png)

## About
Stronghold is a castle sim created by Firefly Studios in 2001. It was released for Windows and supports MacOS since
its HD remaster.
The purpose of this project is to make Stronghold playable on platforms it didn't support originally. It is not intended to be a 100% accurate source port, so there will be differences.

##  Building
After cloning the repository, make sure to run `git submodule init` and `git submodule update` to fetch [cxxopts](https://github.com/jarro2783/cxxopts).

Run `cmake` in your build directory. If you're building on Windows or don't have the libraries in your `pkg-config`,
use the `-DCMAKE_PREFIX_PATH=/custom/path1/;/custom/path2/` option on the command line. Note that you have to give CMake absolute paths. 

Sourcehold depends on the following libraries:
* SDL2
* OpenAL
* FFmpeg (avcodec, avutil, avformat, swscale)
* Boost (filesystem)

## Running
Copy the following files into the `data` folder:  

```
data
├── binks/
├── fx/
├── gfx/
├── gfx8/
├── gm/
├── maps/
├── stronghold.mlb
├── sh.tex
├── delete.ani
├── hand.ani
├── jester.ani
└── sword.ani
```
