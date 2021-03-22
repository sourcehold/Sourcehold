#pragma once
// Disable warnings caused by ghc_filesystem
#if defined(__clang__)
  #if __clang_major__ > 10
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wrange-loop-construct"
  #endif  // __clang_major__ > 10
#endif    // __clang__

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuseless-cast"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wconversion"
#endif  // GCC

#ifdef _MSC_VER
#endif  // _MSC_VER

#include <ghc/filesystem.hpp>

#ifdef __clang__
  #if __clang_major__ > 10
    #pragma GCC diagnostic pop
  #endif  // __clang_major__ > 10
#endif    // __clang__

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
  #pragma GCC diagnostic pop
#endif  // GCC

#ifdef _MSC_VER
// TODO(seidl):
// investigate MSVC warnings
#endif  //_MSC_VER

// undefine macros from ghc_filesystem
#undef ERROR
#undef min
#undef max
