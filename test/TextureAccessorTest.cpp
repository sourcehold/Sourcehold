#include <gtest/gtest.h>
#include <numeric>
#include "SDL/TextureAccessor.h"
#include "Rendering/Shapes.h"
using namespace Sourcehold::SDL;
using namespace Sourcehold::Rendering;

constexpr Vector2<int> size_k{100, 100};

TEST(TextureAccessor, Constructor) {
  auto texture = MakeTextureUQ(SDLBackend::Renderer(), PixelFormat,
                               SDL_TEXTUREACCESS_STREAMING, size_k.x, size_k.y);

  auto accessor = TextureAccessor(texture.get());

  ASSERT_EQ(texture.get(), accessor.texture);
  ASSERT_EQ(PixelFormat, accessor.format);
  ASSERT_EQ(SDL_TEXTUREACCESS_STREAMING, accessor.access);
  ASSERT_EQ(size_k.x, accessor.size.x);
  ASSERT_EQ(size_k.y, accessor.size.y);
  ASSERT_EQ(size_k.x, accessor.pitch / 4);
}

TEST(TextureAccessor, StdAlgorithm) {
  auto texture = MakeTextureUQ(SDLBackend::Renderer(), PixelFormat,
                               SDL_TEXTUREACCESS_STREAMING, size_k.x, size_k.y);

  auto accessor = TextureAccessor(texture.get());

  std::array<Pixel, size_k.x * size_k.y> groundtruth;

  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);
  std::iota(std::begin(accessor), std::end(accessor), 0);

  auto result = std::equal(std::begin(accessor), std::end(accessor),
                           std::begin(groundtruth));
  ASSERT_TRUE(result);
}
