#include "SDLBackend.h"
#include <stdexcept>
#include "System/Logger.h"

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
  renderer_ =
      MakeRendererUQ(window_.get(),  //
                     -1,             //
                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

  SDL_SetRenderDrawBlendMode(renderer_.get(), SDL_BLENDMODE_BLEND);

  const char* hint = "SDL_HINT_RENDER_SCALE_QUALITY";
  if (SDL_SetHintWithPriority(hint, "1", SDL_HINT_OVERRIDE) == SDL_FALSE) {
    System::Logger::warning(System::RENDERING)
        << "Unable to set " << hint << " hint!" << std::endl;
  }
}

SDLBackend::~SDLBackend() {
  SDL_Quit();
}

}  // namespace SDL
}  // namespace Sourcehold
