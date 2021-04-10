#include <gtest/gtest.h>
#include <numeric>
#include "Rendering/Surface.h"
using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

constexpr Vector2<int> size_k = {10, 10};
constexpr Color color = {0xFF, 0xFF, 0xFF};

TEST(Surface, Constructor) {
  Surface surface(size_k);
}

TEST(Surface, Set) {
  Surface surface(size_k);
  surface.Set({0, 0}, color);
  ASSERT_EQ(At<Pixel>(surface.Ptr(), {}), AsPixel(color));
}

TEST(Surface, StdAlgorithm) {
  Surface surface(size_k);
  std::array<Pixel, size_k.x * size_k.y> groundtruth;

  std::iota(std::begin(surface), std::end(surface), 0);
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  auto result = std::equal(std::begin(surface), std::end(surface),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}

TEST(Surface, Fill) {
  Surface surface({10, 10});
  surface.Fill(color);

  for (const auto& pixel : surface) {
    ASSERT_EQ(pixel, AsPixel(color));
  }
}

TEST(Surface, Blit) {
  Surface surface1({5, 5});
  Surface surface2({10, 10});

  std::fill(std::begin(surface1), std::end(surface1), AsPixel({0, 0, 0}));
  std::iota(std::begin(surface2), std::end(surface2), 0);

  std::array<Pixel, 100> groundtruth;
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  for (auto y = 0; y < 5; ++y) {
    for (auto x = 0; x < 5; ++x) {
      groundtruth[At({x, y}, 10)] = 0xFF;
    }
  }

  surface2.Blit(surface1, {0, 0});

  auto result = std::equal(std::begin(surface2), std::end(surface2),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}
TEST(Surface, BlitClipped) {
  Surface surface1({5, 5});
  Surface surface2({10, 10});

  std::fill(std::begin(surface1), std::end(surface1), AsPixel({0, 0, 0}));
  std::iota(std::begin(surface2), std::end(surface2), 0);

  std::array<Pixel, 100> groundtruth;
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  constexpr Vector2<int> pos = {1, 2};
  constexpr Rect<int> clip = {0, 0, 2, 4};

  for (auto y = pos.y; y < clip.h + pos.y; ++y) {
    for (auto x = pos.x; x < clip.w + pos.x; ++x) {
      groundtruth[At({x, y}, 10)] = 0xFF;
    }
  }

  surface2.Blit(surface1, pos, clip);

  auto result = std::equal(std::begin(surface2), std::end(surface2),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}
