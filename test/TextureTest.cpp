#include <gtest/gtest.h>
#include <numeric>
#include "Rendering/Texture.h"
#include "SDL/TextureAccessor.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

TEST(Texture, Copy) {
  TextureStreaming tex1({10, 10});
  TextureStreaming tex2({5, 5});

  {
    auto accessor1 = tex1.Accessor();
    auto accessor2 = tex2.Accessor();

    std::iota(std::begin(accessor1), std::end(accessor1), 0);
    std::fill(std::begin(accessor2), std::end(accessor2), 0xFF);
  }

  tex1.Copy(tex2, {0, 0});

  std::array<Pixel, 100> groundtruth;
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  for (auto y = 0; y < 5; ++y) {
    for (auto x = 0; x < 5; ++x) {
      groundtruth[At({x, y}, 10)] = 0xFF;
    }
  }

  TextureAccessor candidate = tex1.Accessor();

  auto result = std::equal(std::begin(candidate), std::end(candidate),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}

TEST(Texture, CopyClipped) {
  TextureStreaming tex1({10, 10});
  TextureStreaming tex2({5, 5});

  {
    auto accessor1 = tex1.Accessor();
    auto accessor2 = tex2.Accessor();

    std::iota(std::begin(accessor1), std::end(accessor1), 0);
    std::fill(std::begin(accessor2), std::end(accessor2), 0xFF);
  }

  constexpr Vector2<int> pos = {2, 3};
  constexpr Rect<int> clip = {1, 2, 3, 4};
  tex1.Copy(tex2, pos, clip);

  std::array<Pixel, 100> groundtruth;
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  for (auto y = pos.y; y < clip.h + pos.y; ++y) {
    for (auto x = pos.x; x < clip.w + pos.x; ++x) {
      groundtruth[At({x, y}, 10)] = 0xFF;
    }
  }

  TextureAccessor candidate = tex1.Accessor();

  auto result = std::equal(std::begin(candidate), std::end(candidate),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}
