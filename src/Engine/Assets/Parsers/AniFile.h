#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include "Parser.h"

#include "FileSystem.h"

namespace Sourcehold {
namespace Parsers {
/*
 * Animated cursor parser
 */
class AniFile : public Parser {
 public:
  AniFile() = default;
  AniFile(ghc::filesystem::path path);
  ~AniFile() = default;

  bool LoadFromDisk(ghc::filesystem::path path);

 protected:
  bool ParseChunks();
  struct RiffHeader;
  struct RiffChunk;
};
}  // namespace Parsers
}  // namespace Sourcehold
