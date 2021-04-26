#pragma once
#include <cstdint>

namespace Sourcehold {
namespace Rendering {

using Pixel = std::uint32_t;

struct Color {
  uint8_t r, g, b, a;

  constexpr Color(const Color&) = default;
  constexpr Color(Color&&) = default;
  [[nodiscard]] constexpr Color& operator=(const Color&) = default;
  [[nodiscard]] constexpr Color& operator=(Color&&) = default;

  constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) noexcept
      : r(r_), g(g_), b(b_), a(a_){};
  constexpr Color(const Pixel& pixel) noexcept
      : r(Red(pixel)), g(Green(pixel)), b(Blue(pixel)), a(Alpha(pixel)) {
  }
  constexpr Color(Pixel&& pixel) noexcept
      : r(Red(pixel)), g(Green(pixel)), b(Blue(pixel)), a(Alpha(pixel)){};

  [[nodiscard]] constexpr Color& operator=(const Pixel& pixel) noexcept {
    *this = AsColor(pixel);
    return *this;
  }
  [[nodiscard]] constexpr Color& operator=(Pixel&& pixel) noexcept {
    *this = AsColor(pixel);
    return *this;
  }

  operator Pixel() const {
    return AsPixel(*this);
  }

  ~Color() = default;

 private:
  [[nodiscard]] constexpr Pixel AsPixel(Color color) const noexcept {
    return static_cast<Pixel>(color.r) << 24 |  //
           static_cast<Pixel>(color.g) << 16 |  //
           static_cast<Pixel>(color.b) << 8 |   //
           static_cast<Pixel>(color.a);
  }
  [[nodiscard]] constexpr uint8_t Red(Pixel pixel) const noexcept {
    return static_cast<uint8_t>(pixel >> 24);
  }
  [[nodiscard]] constexpr uint8_t Green(Pixel pixel) const noexcept {
    return static_cast<uint8_t>((pixel >> 16) & 0xFF);
  }
  [[nodiscard]] constexpr uint8_t Blue(Pixel pixel) const noexcept {
    return static_cast<uint8_t>((pixel >> 8) & 0xFF);
  }
  [[nodiscard]] constexpr uint8_t Alpha(Pixel pixel) const noexcept {
    return static_cast<uint8_t>(pixel & 0xFF);
  }
  [[nodiscard]] constexpr Color AsColor(Pixel pixel) const noexcept {
    return {Red(pixel),    //
            Green(pixel),  //
            Blue(pixel),   //
            Alpha(pixel)};
  }
};

struct ColorMask {
  Pixel r, g, b, a;
};

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

namespace ColorMasks {
constexpr auto RGBA8888 = ColorMask{0xFF000000, 0xFF0000, 0xFF00, 0xFF};
}
}  // namespace Rendering
}  // namespace Sourcehold
