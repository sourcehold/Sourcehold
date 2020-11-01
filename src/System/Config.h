#pragma once

#define SOURCEHOLD_VERSION_MAJOR 0
constexpr int VERSION_MAJOR = 0;
#define SOURCEHOLD_VERSION_MINOR 1
constexpr int VERSION_MINOR = 1;
#define SOURCEHOLD_VERSION_STRING "0.1.4"
constexpr const char* VERSION_STRING = "0.1.4";

// If Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(win32)

/// If not using MSVC compiler
#if defined(__GNUC__) || defined(__MINGW32__)
#define SOURCEHOLD_BUILD "MinGW"
constexpr const char* BUILD = "MinGW";
#define SOURCEHOLD_MINGW 1
constexpr bool MINGW = true;
#define ATTRIB_PACKED __attribute__((packed))

/// Else MSVC
#else
#define SOURCEHOLD_BUILD "Windows"
constexpr const char* BUILD = "Windows";
#define SOURCEHOLD_WINDOWS 1
constexpr bool WINDOWS = true;
// How do we handle not defining packed?
#define ATTRIB_PACKED
#endif

// Else if Linux
#elif defined(linux) || defined(__linux)
constexpr bool WINDOWS = false;
constexpr bool MINGW = false;

#define SOURCEHOLD_BUILD "Linux"
constexpr const char* BUILD = "Linux";
#define SOURCEHOLD_LINUX 1
constexpr bool LINUX = true;
#define SOURCEHOLD_UNIX 1
constexpr bool UNIX = true;
#define ATTRIB_PACKED __attribute__((packed))

// If Android
#if defined(__ANDROID__)
#define SOURCEHOLD_BUILD "Android"
constexpr const char* BUILD = "Android";
#define SOURCEHOLD_ANDROID 1
constexpr bool ANDROID = true;
#endif
constexpr bool ANDROID = false;

// Else Generic Unix build
#else
constexpr bool WINDOWS = false;
constexpr bool MINGW = false;
constexpr bool LINUX = false;

#define SOURCEHOLD_BUILD "Generic unix"
constexpr const char* BUILD = "Generic Unix";
#define SOURCEHOLD_UNIX 1
constexpr bool UNIX = true;
#define ATTRIB_PACKED __attribute__((packed))
#endif
// Endif Platform

// Configuration of hardcoded stuff
//
// Render the menu border for the loading screen
#define RENDER_LOADING_BORDER 1
constexpr bool RENDER_LOADING_BORDER_EX = true;

// Scale the main menu to fit the screen in classic edition
#define SCALE_MAIN_MENU 0
constexpr bool SCALE_MAIN_MENU_EX = false;

// Invert camera on Android
#define INVERT_CAM_ON_ANDROID 1
constexpr bool INVERT_CAM_ON_ANDROID_EX = true;

