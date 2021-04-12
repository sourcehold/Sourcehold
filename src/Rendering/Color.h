#pragma once
#include <cstdint>

namespace Sourcehold {
namespace Rendering {
struct Color {
  uint8_t r, g, b;
  uint8_t a = 255;
};

using Pixel = std::uint32_t;

[[nodiscard]] constexpr inline Pixel AsPixel(Color color) noexcept {
  return static_cast<uint32_t>(color.r) << 24 |  //
         static_cast<uint32_t>(color.g) << 16 |  //
         static_cast<uint32_t>(color.b) << 8 |   //
         static_cast<uint32_t>(color.a);
}
[[nodiscard]] constexpr inline Color AsColor(Pixel pixel) noexcept {
  return {static_cast<uint8_t>(pixel >> 24),           //
          static_cast<uint8_t>((pixel >> 16) & 0xFF),  //
          static_cast<uint8_t>((pixel >> 8) & 0xFF),   //
          static_cast<uint8_t>(pixel & 0xFF)};
}

namespace Colors {
constexpr auto Black = Color{0, 0, 0};
constexpr auto White = Color{255, 255, 255};
constexpr auto Red = Color{255, 0, 0};
constexpr auto Green = Color{0, 255, 0};
constexpr auto Blue = Color{0, 0, 255};
constexpr auto Yellow = Color{255, 255, 0};
constexpr auto Cyan = Color{0, 255, 255};
constexpr auto Magenta = Color{255, 0, 255};
}  // namespace Colors
}  // namespace Rendering
}  // namespace Sourcehold
