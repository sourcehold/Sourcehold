Sourcehold [![Build Status](https://travis-ci.org/metalvoidzz/Sourcehold.svg?branch=master)](https://travis-ci.org/metalvoidzz/Sourcehold)
======

Open source engine implementation of Stronghold by Firefly Studios. This project is currently in an early stage of development. Once completed, it should be able to replace all editions of classic Stronghold, including the HD remaster.

---

![Imgur](https://i.imgur.com/jWMp8Qx.png)

##  Building
After cloning the repository, make sure to run `git submodule init` and `git submodule update` to fetch [cxxopts](https://github.com/jarro2783/cxxopts).

Run `cmake` in your build directory. If you're building on Windows or don't have the libraries in your `pkg-config`,
use the `-DCMAKE_PREFIX_PATH=/custom/path/` option on the command line.

Sourcehold depends on the following libraries:
* SDL2
* OpenAL
* FFmpeg
* Boost

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
├── delete.ani
├── hand.ani
├── jester.ani
└── sword.ani
```
