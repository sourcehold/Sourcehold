#pragma once

#include <SDL.h>
#include <cassert>
#include <memory>
#include "Rendering/Shapes.h"

namespace Sourcehold {
namespace SDL {
namespace Details {
struct SDL_Deleter {
  void operator()(SDL_Surface* surface) {
    SDL_FreeSurface(surface);
  }
  void operator()(SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
  }
  void operator()(SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
  }
  void operator()(SDL_Window* window) {
    SDL_DestroyWindow(window);
  }
};

struct ColorMask {
  uint32_t r, g, b, a;
};
}  // namespace Details

using SDL_Surface_UQ = std::unique_ptr<SDL_Surface, Details::SDL_Deleter>;
using SDL_Texture_UQ = std::unique_ptr<SDL_Texture, Details::SDL_Deleter>;
using SDL_Renderer_UQ = std::unique_ptr<SDL_Renderer, Details::SDL_Deleter>;
using SDL_Window_UQ = std::unique_ptr<SDL_Window, Details::SDL_Deleter>;

using SDL_Surface_SH = std::shared_ptr<SDL_Surface>;
using SDL_Texture_SH = std::shared_ptr<SDL_Texture>;
using SDL_Renderer_SH = std::shared_ptr<SDL_Renderer>;
using SDL_Window_SH = std::shared_ptr<SDL_Window>;

constexpr int SurfaceDepth = 32;
constexpr Details::ColorMask SurfaceMask{0xFF000000,  //
                                         0x00FF0000,  //
                                         0x0000FF00,  //
                                         0x000000FF};
constexpr auto BlendMode = SDL_BLENDMODE_BLEND;

constexpr auto PixelFormat = SDL_PIXELFORMAT_RGBA8888;

[[nodiscard]] inline int At(Rendering::Vector2<int> pos, int w) {
  return pos.x + pos.y * w;
}

template <typename T>
[[nodiscard]] T& At(SDL_Surface* surf, Rendering::Vector2<int> pos) {
  static_assert(std::is_arithmetic_v<T>);
  T* ptr = static_cast<T*>(surf->pixels);
  return ptr[At(pos, surf->w)];
}

template <typename T>
[[nodiscard]] T& AtSafe(SDL_Surface* surf, Rendering::Vector2<int> pos) {
  static_assert(std::is_arithmetic_v<T>);

  assert(surf->w > pos.x);
  assert(surf->h > pos.y);

  return At<T>(surf, pos);
}

[[nodiscard]] inline SDL_Rect* ToSDLRectPtr(Rendering::Rect<int>& rect) {
  using namespace Rendering;
  static_assert(std::is_same_v<decltype(Rect<int>::x), decltype(SDL_Rect::x)>);
  static_assert(std::is_same_v<decltype(Rect<int>::y), decltype(SDL_Rect::y)>);
  static_assert(std::is_same_v<decltype(Rect<int>::w), decltype(SDL_Rect::w)>);
  static_assert(std::is_same_v<decltype(Rect<int>::h), decltype(SDL_Rect::h)>);
  static_assert(sizeof(Rect<int>) == sizeof(SDL_Rect));
  static_assert(sizeof(Rect<int>*) == sizeof(SDL_Rect*));

  return reinterpret_cast<SDL_Rect*>(&rect);
}

}  // namespace SDL
}  // namespace Sourcehold
