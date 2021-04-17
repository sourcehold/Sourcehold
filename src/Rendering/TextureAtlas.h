#pragma once

#include <cinttypes>
#include <memory>
#include <utility>
#include <vector>

#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

namespace Sourcehold {
namespace Rendering {
struct TextureAtlasCreateInfo {
  Surface surface;
  std::vector<Rect<int>> entries;
};
/**
 * Merge multiple textures. First, give it a list of image dimensions
 * to handle, then use the offsets returned by Get() to write to the
 * intermediate surface. When done, call Create() to convert the surface
 * to a static texture.
 */
class TextureAtlas : public TextureStatic {
 public:
  TextureAtlas() = delete;
  TextureAtlas(const TextureAtlasCreateInfo& atlas);
  ~TextureAtlas() = default;

  static TextureAtlasCreateInfo CreateInfo(
      std::vector<Vector2<int>>& dimensions);

  Rect<int> Get(size_t index);
  std::vector<Rect<int>> entries_;

 protected:
  constexpr static auto MAX_X_RESOLUTION = 2048;
};
}  // namespace Rendering
}  // namespace Sourcehold
