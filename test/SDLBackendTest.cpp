#include <gtest/gtest.h>
#include "SDL/SDLBackend.h"
using namespace Sourcehold::SDL;
using namespace Sourcehold::Rendering;

TEST(SDLBackend, Init) {
  ASSERT_NO_THROW(SDLBackend::Instance());
  ASSERT_NO_THROW(SDLBackend::Renderer());
  ASSERT_NO_THROW(SDLBackend::Window());

  ASSERT_NE(SDLBackend::Renderer(), nullptr);
  ASSERT_NE(SDLBackend::Window(), nullptr);
}

TEST(SDLHelpers, At) {
  Vector2<int> pos = {4, 6};
  auto width = 3;

  // 6 * 3 + 4 = 22
  auto groundtruth = 22;

  ASSERT_EQ(At(pos, width), groundtruth);
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
