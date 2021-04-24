#pragma once

#include <SDL.h>
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
}  // namespace SDL
}  // namespace Sourcehold
