#pragma once
#include <cstdint>

namespace Sourcehold {
namespace Rendering {
struct Color {
  uint8_t r, g, b;
  uint8_t a = 255;
};

[[nodiscard]] inline uint32_t AsPixel(Color color) noexcept {
  return static_cast<uint32_t>(color.r) << 24 |  //
         static_cast<uint32_t>(color.g) << 16 |  //
         static_cast<uint32_t>(color.b) << 8 |   //
         static_cast<uint32_t>(color.a);
}
[[nodiscard]] inline Color AsColor(uint32_t pixel) noexcept {
  return {static_cast<uint8_t>(pixel >> 24),           //
          static_cast<uint8_t>((pixel >> 16) & 0xFF),  //
          static_cast<uint8_t>((pixel >> 8) & 0xFF),   //
          static_cast<uint8_t>(pixel & 0xFF)};
}
}  // namespace Rendering
}  // namespace Sourcehold
