#pragma once

#include <cinttypes>
#include "System/filesystem.h"

#include "Parsers/Parser.h"

#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

namespace Sourcehold {
namespace Parsers {
using namespace Rendering;

class MapFile : private Parser {
  Texture preview;

 public:
  MapFile();
  MapFile(ghc::filesystem::path path);
  MapFile(const MapFile&) = delete;
  ~MapFile();

  bool LoadFromDisk(ghc::filesystem::path path);

  inline Texture& GetPreview() {
    return preview;
  }

 protected:
  struct MapSec;
  MapSec BlastSection();
  void ParsePreview();
  uint32_t ComputeCRC32(const void* data, size_t size);
};
}  // namespace Parsers
}  // namespace Sourcehold
