#include "Rendering/Tileset.h"
#include "System/Logger.h"

#include <cmath>
#include <utility>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Tileset::Tileset(const TilesetCreateInfo &create_info)
    : TextureStreaming(create_info.surface),
      num_of_tiles_(create_info.num_of_tiles) {
}
TilesetCreateInfo Tileset::CreateInfo(size_t num_of_tiles) {
  auto num_rows =
      static_cast<int>(std::sqrt(static_cast<float>(num_of_tiles)) + 0.5f);
  return {Surface(Vector2<int>{num_rows, num_rows} * tile_size),  //
          num_of_tiles, static_cast<size_t>(num_rows)};
}

void Tileset::SetTile(TextureStreaming &image, size_t index) {
  auto coords = IndexToCoords(index);

  Texture::Copy(image, coords);
}

Rect<int> Tileset::GetTile(size_t index) {
  auto coords = IndexToCoords(index);

  return {coords.x, coords.y, tile_size.x, tile_size.y};
}

Vector2<int> Tileset::IndexToCoords(size_t index) {
  return {tile_size.x * static_cast<int>(index % num_of_rows),
          tile_size.y * static_cast<int>(index / num_of_rows)};
}
