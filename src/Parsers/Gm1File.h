#pragma once

#include <cinttypes>
#include <memory>

#include "Rendering/Texture.h"
#include "Rendering/Tileset.h"
#include "Rendering/TextureAtlas.h"
#include "Rendering/Renderer.h"

#include "Parsers/Parser.h"

#include "System/filesystem.h"

namespace Sourcehold {
namespace Parsers {
using namespace Rendering;

/**
 * Container format for tiles/animations/images
 * Creates an entry in TextureAtlas for every image collection (Building, unit,
 * etc.) and a Tileset depending on the stored type
 */
class Gm1File : private Parser {
  std::shared_ptr<TextureAtlas> textureAtlas;
  std::shared_ptr<Tileset> tileset;
  struct ImageHeader;
  struct Gm1Entry;
  struct Gm1Header;

 public:
  Gm1File();
  Gm1File(ghc::filesystem::path path, bool cached = false);
  Gm1File(const Gm1File &) = delete;
  Gm1File &operator=(const Gm1File &) = delete;
  ~Gm1File();

  bool LoadFromDisk(ghc::filesystem::path path, bool cache = false);
  void Free();

  inline std::shared_ptr<Tileset> GetTileset() {
    return tileset;
  }
  inline std::shared_ptr<TextureAtlas> GetTextureAtlas() {
    return textureAtlas;
  }

 protected:
  const uint32_t MAX_NUM = 2048;
  bool GetImage(uint32_t index, std::vector<Gm1Entry> &entries, char *imgdata,
                Gm1Header *header);
  void ReadPalette();
};
}  // namespace Parsers
}  // namespace Sourcehold
