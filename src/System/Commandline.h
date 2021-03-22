#pragma once
#if defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif  // __clang__

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wconversion"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif  // GCC

#if defined(_MSC_VER)
#endif  // _MSC_VER

#include <cxxopts.hpp>

#if defined(__clang__)
  #pragma GCC diagnostic pop
#endif  // __clang__

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
  #pragma GCC diagnostic pop
#endif  // GCC

#if defined(_MSC_VER)
#endif  // _MSC_VER
