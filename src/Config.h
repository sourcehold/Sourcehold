#pragma once

#define OPENSH_VERSION_MAJOR 0
#define OPENSH_VERSION_MINOR 1
#define OPENSH_VERSION_STRING "0.1"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(win32)
#define OPENSH_BUILD "Windows"
#define OPENSH_WINDOWS 1
#elif defined(linux) || defined(__linux)
#define OPENSH_BUILD "Linux"
#define OPENSH_LINUX 1
#define OPENSH_UNIX 1
#else /* assume unix */
#define OPENSH_BUILD "Generic unix"
#define OPENSH_UNIX 1
#endif
