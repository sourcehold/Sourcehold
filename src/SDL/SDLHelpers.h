#pragma once

#include <SDL2/SDL.h>
#include <memory>

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
}  // namespace Details

using SDL_Surface_UQ = std::unique_ptr<SDL_Surface, Details::SDL_Deleter>;
using SDL_Texture_UQ = std::unique_ptr<SDL_Texture, Details::SDL_Deleter>;
using SDL_Renderer_UQ = std::unique_ptr<SDL_Renderer, Details::SDL_Deleter>;
using SDL_Window_UQ = std::unique_ptr<SDL_Window, Details::SDL_Deleter>;

using SDL_Surface_SH = std::shared_ptr<SDL_Surface>;
using SDL_Texture_SH = std::shared_ptr<SDL_Texture>;
using SDL_Renderer_SH = std::shared_ptr<SDL_Renderer>;
using SDL_Window_SH = std::shared_ptr<SDL_Window>;

struct ColorMask {
  uint32_t r, g, b, a;
};

constexpr int SurfaceDepth = 32;
constexpr ColorMask SurfaceMask{0xFF000000,  //
                                0x00FF0000,  //
                                0x0000FF00,  //
                                0x000000FF};
constexpr auto BlendMode = SDL_BLENDMODE_BLEND;

template <typename T>
[[nodiscard]] T& At(SDL_Surface* surf, int x, int y) {
  static_assert(std::is_arithmetic_v<T>);
  T* ptr = static_cast<T*>(surf->pixels)[y * surf->w + x];
  return *ptr;
}

}  // namespace SDL
}  // namespace Sourcehold
