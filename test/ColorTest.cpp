#include <gtest/gtest.h>
#include "Rendering/Color.h"
using namespace Sourcehold::Rendering;

constexpr Color color = {0xFF, 0xAF, 0xC0, 0x42};
constexpr Pixel pixel = 0xFFAFC042;

TEST(Color, PixelToColor) {
  const auto candidate = Color(pixel);
  ASSERT_EQ(color.r, candidate.r);
  ASSERT_EQ(color.g, candidate.g);
  ASSERT_EQ(color.b, candidate.b);
  ASSERT_EQ(color.a, candidate.a);
}

TEST(Color, ColorToPixel) {
  ASSERT_EQ(pixel, static_cast<Pixel>(color));
}
