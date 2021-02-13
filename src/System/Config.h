#pragma once

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#define SOURCEHOLD_VERSION_MAJOR 0
#define SOURCEHOLD_VERSION_MINOR 1
#define SOURCEHOLD_VERSION_STRING "0.1.4"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(win32)

#if defined(__GNUC__) || defined(__MINGW32__)
#define SOURCEHOLD_BUILD "MinGW"
#define SOURCEHOLD_MINGW 1
#define ATTRIB_PACKED __attribute__((packed))
#else
#define SOURCEHOLD_BUILD "Windows"
#define SOURCEHOLD_WINDOWS 1
#define ATTRIB_PACKED
#endif

#elif defined(linux) || defined(__linux)

#define SOURCEHOLD_BUILD "Linux"
#define SOURCEHOLD_LINUX 1
#define SOURCEHOLD_UNIX 1
#define ATTRIB_PACKED __attribute__((packed))
#if defined( __ANDROID__)
#define SOURCEHOLD_BUILD "Android"
#define SOURCEHOLD_ANDROID 1
#endif

#elif TARGET_OS_MAC

#if TARGET_OS_IOS

#define SOURCEHOLD_BUILD "iOS"
#define SOURCEHOLD_iOS 1

#elif TARGET_OS_OSX

#define SOURCEHOLD_BUILD "Mac OS"
#define SOURCEHOLD_MAC_OS 1

#else

#error "Unsupported platform"

#endif // TARGET_OS_OSX

#define SOURCEHOLD_UNIX 1
#define ATTRIB_PACKED __attribute__((packed))

#else /* assume unix */

#define SOURCEHOLD_BUILD "Generic unix"
#define SOURCEHOLD_UNIX 1
#define ATTRIB_PACKED __attribute__((packed))

#endif

/* Configuration of hardcoded stuff */

#define RENDER_LOADING_BORDER 1 // Render the menu border for the loading screen
#define SCALE_MAIN_MENU 0  // Scale the main menu to fit the screen in classic edition
#define INVERT_CAM_ON_ANDROID 1
