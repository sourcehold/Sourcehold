#pragma once

#include <optional>

#include "SDL/SDLHelpers.h"
#include "Common/Shapes.h"
#include "Color.h"

namespace Sourcehold {
namespace Rendering {

// ----------------------------------------------------------------------------
// Surface  -- Prototype
// ----------------------------------------------------------------------------
class Surface {
 public:
  constexpr static auto Mask = ColorMasks::RGBA8888;
  constexpr static auto Depth = 32;
  constexpr static auto BlendMode = SDL_BLENDMODE_BLEND;

  Surface() : surface_{nullptr} {};
  Surface(const Surface& other);
  Surface(Surface&& other);
  Surface(Vector2<int> size);
  Surface& operator=(const Surface& other);
  Surface& operator=(Surface&& other);
  ~Surface() = default;

  void Set(Vector2<int> pos, Color color) const noexcept;
  void Blit(const Surface& other, Vector2<int> pos,
            std::optional<Rect<int>> clip = {}) const noexcept;
  void Fill(Color color) const noexcept;

  [[nodiscard]] operator SDL_Surface*() const noexcept {
    return surface_.get();
  }

  [[nodiscard]] Pixel* begin() const noexcept;
  [[nodiscard]] Pixel* end() const noexcept;
  [[nodiscard]] const Pixel* cbegin() const noexcept;
  [[nodiscard]] const Pixel* cend() const noexcept;

 private:
  SDL::SDL_Surface_UQ surface_;
};
}  // namespace Rendering
}  // namespace Sourcehold
