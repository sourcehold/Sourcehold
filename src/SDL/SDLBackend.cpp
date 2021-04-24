#include "SDLBackend.h"
#include <stdexcept>

namespace Sourcehold {
namespace SDL {

SDLBackend::SDLBackend() {
  auto result = SDL_Init(SDL_INIT_VIDEO);

  if (result < 0) {
    throw std::runtime_error("Failed to init SDL2");
  }

  window_ = MakeWindowUQ(nullptr,                                           //
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  //
                         0, 0,                                              //
                         0);
  if (!window_) {
    throw std::runtime_error("Failed to create SDL_Window");
  }

  renderer_ =
      MakeRendererUQ(window_.get(),  //
                     -1,             //
                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

  if (!renderer_) {
    throw std::runtime_error("Failed to create SDL_Renderer");
  }
}

SDLBackend::~SDLBackend() {
  SDL_Quit();
}

}  // namespace SDL
}  // namespace Sourcehold
