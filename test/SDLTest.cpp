#include <gtest/gtest.h>
#include <numeric>
#include <array>
#include "SDL/SDLBackend.h"
using namespace Sourcehold::SDL;

TEST(SDLBackend, Init) {
  ASSERT_NO_THROW(SDLBackend::Instance());
  ASSERT_NO_THROW(SDLBackend::Renderer());
  ASSERT_NO_THROW(SDLBackend::Window());

  ASSERT_NE(SDLBackend::Renderer(), nullptr);
  ASSERT_NE(SDLBackend::Window(), nullptr);
}

TEST(SDLHelpers, At) {
  std::array<int, 100> groundtruth;
  std::iota(std::begin(groundtruth), std::end(groundtruth), 0);

  for (auto y = 0; y < 10; ++y) {
    for (auto x = 0; x < 10; ++x) {
      ASSERT_EQ(At({x, y}, 10), groundtruth[At({x, y}, 10)]);
    }
  }
}

TEST(SDLHelpers, AtSafe) {
  Vector2<int> pos = {3, 3};

  auto surface = MakeSurfaceUQ(0, 2, 2, 32, SDL_PIXELFORMAT_RGBA4444);

  ASSERT_DEATH(AtSafe<uint32_t>(surface.get(), pos) = 3, "");
}

TEST(SDLHelpers, ToSDLRectPtr) {
  Rect<int> rect = {1, 2, 3, 4};
  auto sdl_rect = ToSDLRectPtr(rect);

  ASSERT_EQ(static_cast<void*>(sdl_rect), static_cast<void*>(&rect));
  ASSERT_EQ(sdl_rect->x, rect.x);
  ASSERT_EQ(sdl_rect->y, rect.y);
  ASSERT_EQ(sdl_rect->w, rect.w);
  ASSERT_EQ(sdl_rect->h, rect.h);
}
