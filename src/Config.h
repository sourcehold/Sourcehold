#pragma once

#define SOURCEHOLD_VERSION_MAJOR 0
#define SOURCEHOLD_VERSION_MINOR 1
#define SOURCEHOLD_VERSION_STRING "0.1"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(win32)
#define SOURCEHOLD_BUILD "Windows"
#define SOURCEHOLD_WINDOWS 1
#elif defined(linux) || defined(__linux)
#define SOURCEHOLD_BUILD "Linux"
#define SOURCEHOLD_LINUX 1
#define SOURCEHOLD_UNIX 1
#else /* assume unix */
#define SOURCEHOLD_BUILD "Generic unix"
#define SOURCEHOLD_UNIX 1
#endif
