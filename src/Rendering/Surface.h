#pragma once

#include "SDL/SDLBackend.h"
#include "Shapes.h"
#include "Color.h"

namespace Sourcehold {
namespace Rendering {

// ----------------------------------------------------------------------------
// Surface  -- Prototype
// ----------------------------------------------------------------------------
class Surface {
 public:
  Surface() = delete;
  Surface(Vector2<int> size);
  ~Surface() = default;

  void Lock() const noexcept;
  void Unlock() const noexcept;
  void Set(Vector2<int> pos, Color color) const noexcept;
  void Blit(const Surface& other, Vector2<int> pos,
            std::optional<Rect<int>> clip) const noexcept;
  void Fill(Color color) const noexcept;

  [[nodiscard]] SDL_Surface* Ptr() const noexcept {
    return surface_.get();
  }

 private:
  SDL::SDL_Surface_UQ surface_;
};
}  // namespace Rendering
}  // namespace Sourcehold
