#include "VideoEngine.h"
#include "Logger.h"
#include "Config.h"
#include "GameOptions.h"
#include <SDL.h>
#include <SDL_error.h>
#include <algorithm>

void VideoEngine::InitSDL() {
  using namespace Sourcehold::System;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
}

static void checkResolution(const Resolution& resolution) noexcept {
  using namespace Sourcehold::System;

  if (std::find(std::begin(available_resolutions),
                std::end(available_resolutions),
                resolution) == std::end(available_resolutions)) {
    Logger::warning(GAME) << "You selected an unsupported resolution. This may "
                             "lead to ugly scaling!"
                          << std::endl;
  }
}
static uint32_t getWindowParameters() noexcept {
  uint32_t param = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS;
  if (game_options_gk.fullscreen_) {
    param |= SDL_WINDOW_FULLSCREEN;
  }
  if (game_options_gk.noborder_) {
    param |= SDL_WINDOW_BORDERLESS;
  }
  return param;
}

void VideoEngine::CreateWindow() {
  using namespace Sourcehold::System;

  checkResolution(game_options_gk.resolution_);

  const auto window_parameters = getWindowParameters();

  window_ = SDL_CreateWindow(  //
      "Sourcehold version " SOURCEHOLD_VERSION_STRING " - " SOURCEHOLD_BUILD,
      SDL_WINDOWPOS_CENTERED,              //
      SDL_WINDOWPOS_CENTERED,              //
      game_options_gk.resolution_.width,   //
      game_options_gk.resolution_.height,  //
      window_parameters);

  if (window_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  } else {
    if (!game_options_gk.nograb_) {
      SDL_SetWindowGrab(window_, SDL_TRUE);
    }
  }
}

static void setRendererHint() {
  using namespace Sourcehold::System;
  if (SDL_SetHintWithPriority(              //
          "SDL_HINT_RENDER_SCALE_QUALITY",  //
          "1",                              //
          SDL_HINT_OVERRIDE) == SDL_FALSE) {
    Logger::warning(RENDERING)
        << "Unable to set SDL_HINT_RENDER_SCALE_QUALITY" << std::endl;
  }
}

void VideoEngine::CreateRenderer() {
  renderer_ = SDL_CreateRenderer(  //
      window_,                     //
      -1,                          //
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

  if (renderer_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  } else {
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    setRendererHint();
  }
}

VideoEngine::VideoEngine() {
  using namespace Sourcehold::System;
  Logger::message(RENDERING) << "Creating Video Engine .. " << std::endl;
  try {
    InitSDL();
    CreateWindow();
    CreateRenderer();
  } catch (std::exception& e) {
    Logger::error(RENDERING) << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

VideoEngine::~VideoEngine() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
