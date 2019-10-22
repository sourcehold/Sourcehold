#pragma once

/**
 * Wrapper file for ghc::filesystem, because screw you, whoever
 * decided defining macros with generic names like this in a library
 * would be a good idea!
 */
#include <ghc/filesystem.hpp>

#undef ERROR
#undef min
#undef max
