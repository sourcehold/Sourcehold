#pragma once

#include <cinttypes>
#include <memory>

#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Surface.h"
#include "Rendering/Renderer.h"

namespace Sourcehold {
namespace Rendering {
using namespace Rendering;
struct TilesetCreateInfo {
  Surface surface;
  size_t num_of_tiles;
  size_t num_of_rows;
};
/*
 * Constructs a tileset from a GM1 container
 */
class Tileset : public TextureStreaming {
 public:
  Tileset(const TilesetCreateInfo &create_info);

  static TilesetCreateInfo CreateInfo(size_t num_of_tiles);

  void SetTile(TextureStreaming &image, size_t index);
  Rect<int> GetTile(size_t index);

  size_t num_of_tiles_;

 private:
  Vector2<int> IndexToCoords(size_t index);

  size_t num_of_rows;
  constexpr static auto tile_size = Vector2<int>{30, 16};
};
}  // namespace Rendering
}  // namespace Sourcehold
