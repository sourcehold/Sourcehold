#include "Rendering/TextureAtlas.h"

#include <cmath>
#include <algorithm>

using namespace Sourcehold::Rendering;

TextureAtlasCreateInfo TextureAtlas::CreateInfo(
    std::vector<Vector2<int>>& dimensions) {
  struct Elem {
    Vector2<int> dim;
    size_t index;
  };

  std::vector<Elem> sorted(dimensions.size());
  for (size_t i = 0; i < sorted.size(); i++) {
    Elem elem;
    elem.index = i;
    elem.dim = dimensions[i];
    sorted[i] = elem;
  }

  std::sort(sorted.begin(), sorted.end(), [](Elem& e1, Elem& e2) -> bool {
    return e1.dim.x > e2.dim.x;
  });

  std::vector<Rect<int>> entries(dimensions.size());

  auto width = 0, height = 0;
  auto px = 0, py = 0;
  for (size_t i = 0; i < sorted.size(); i++) {
    Elem e = sorted[i];

    if (px > MAX_X_RESOLUTION) {
      px = 0;
      py = height;
    }

    Rect<int> rect;
    rect.x = px;
    rect.y = py;
    rect.w = e.dim.x;
    rect.h = e.dim.y;

    px += e.dim.x;

    width = std::max(width, px);
    height = std::max(height, py + e.dim.y);

    entries[e.index] = rect;
  }

  return {Surface({width, height}), entries};
}

TextureAtlas::TextureAtlas(const TextureAtlasCreateInfo& create_info)
    : Texture(create_info.surface), entries_(create_info.entries) {
}

Rect<int> TextureAtlas::Get(size_t index) {
  if (index < entries_.size())
    return entries_[index];
  else
    return {0, 0, 0, 0};
}
