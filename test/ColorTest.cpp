#include <gtest/gtest.h>
#include "Rendering/Color.h"
using namespace Sourcehold::Rendering;

constexpr Color color = {0xFF, 0xFF, 0xFF, 0xFF};
constexpr Pixel pixel = 0xFFFFFFFF;

TEST(Color, AsColor) {
  const auto candidate = AsColor(pixel);
  ASSERT_EQ(color.r, candidate.r);
  ASSERT_EQ(color.g, candidate.g);
  ASSERT_EQ(color.b, candidate.b);
  ASSERT_EQ(color.a, candidate.a);
}

TEST(Color, AsPixel) {
  ASSERT_EQ(pixel, AsPixel(color));
}
