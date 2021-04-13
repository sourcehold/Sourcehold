#pragma once

#include "Rendering/Texture.h"
#include "Rendering/Shapes.h"
#include "Rendering/Resolution.h"

namespace Sourcehold {
namespace Rendering {
class Renderer {
 public:
  static void Clear() noexcept;
  static void Flush() noexcept;
  template <class TextureInfo>
  static void Draw(const Texture<TextureInfo> &texture, Vector2<int> dest,
                   std::optional<Rect<int>> clip = {}) noexcept;
  template <class TextureInfo>
  static void Draw(const Texture<TextureInfo> &texture, Rect<int> dest,
                   std::optional<Rect<int>> clip = {}) noexcept;
  template <class TextureInfo>
  static void Draw(const Texture<TextureInfo> &texture,
                   std::optional<Rect<int>> clip = {}) noexcept;
  static void Draw(Rect<int> rect, Color color = Colors::Black,
                   bool fill = false) noexcept;
  static void Draw(Line<int> line, Color color = Colors::White) noexcept;
  static void Fill(Color color) noexcept;

 private:
  static void SetDrawColor(Color color) noexcept;
};
}  // namespace Rendering
}  // namespace Sourcehold
