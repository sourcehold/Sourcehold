#pragma once

#include <cinttypes>
#include <algorithm>
#include <bitset>
#include <memory>

#include "Parser.h"

#include "System/Logger.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

#include "System/filesystem.h"

using namespace Sourcehold::Rendering;

namespace Sourcehold {
namespace Parsers {
using Rendering::Texture;

/*
 * File containing a single image, provides parsing functions
 * for GM1 files too
 */
class TgxFile : private Parser, public Texture {
 public:
  TgxFile();
  TgxFile(ghc::filesystem::path path);
  ~TgxFile();

  bool LoadFromDisk(ghc::filesystem::path path);
  void Unload();

  static void ReadTgx(Surface &tex, char *buf, size_t size, uint16_t offX,
                      uint16_t offY, uint16_t *pal = NULL, uint8_t color = 0);
  static void ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b,
                        uint8_t &a);

 protected:
  struct TgxHeader;
};
}  // namespace Parsers
}  // namespace Sourcehold
