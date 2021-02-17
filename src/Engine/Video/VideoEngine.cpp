#include "VideoEngine.h"
#include "System/Logger.h"
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
static int getWindowParameters(const GameOptions& options) noexcept {
  auto param = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS;
  if (options.fullscreen_) {
    param |= SDL_WINDOW_FULLSCREEN;
  }
  if (options.noborder_) {
    param |= SDL_WINDOW_BORDERLESS;
  }
  return param;
}

static int checkDisplayIndex(int requested_index) noexcept {
  const auto number_of_displays = SDL_GetNumVideoDisplays();
  return number_of_displays < requested_index ? 0 : requested_index;
}

// used to center the window
// SDL_WINDOWPOS_CENTERED should suffice
static SDL_Rect getDisplayBounds(const GameOptions& options) noexcept {
  auto display_bounds = SDL_Rect{};

  SDL_GetDisplayBounds(checkDisplayIndex(options.monitor_index_),
                       &display_bounds);

#ifdef SOURCEHOLD_ANDROID
  // On Android, only use the native resolution
  SDL_DisplayMode mode;
  SDL_GetCurrentDisplayMode(index, &mode);
  // Landscape orientation
  display_bounds.w = mode.h;
  display_bounds.h = mode.w;
#endif  // SOURCEHOLD_ANDROID

  return display_bounds;
}

void VideoEngine::CreateWindow(const GameOptions& options) {
  using namespace Sourcehold::System;

  checkResolution(options.resolution_);

  const auto window_parameters = getWindowParameters(options);

  window_ = SDL_CreateWindow(  //
      "Sourcehold version " SOURCEHOLD_VERSION_STRING " - " SOURCEHOLD_BUILD,
      SDL_WINDOWPOS_CENTERED,      //
      SDL_WINDOWPOS_CENTERED,      //
      options.resolution_.width,   //
      options.resolution_.height,  //
      window_parameters);

  if (window_ == nullptr) {
    throw std::runtime_error(SDL_GetError());
  } else {
    if (!options.nograb_) {
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

VideoEngine::VideoEngine(const GameOptions& options) {
  using namespace Sourcehold::System;
  Logger::message(RENDERING) << "Creating Video Engine .. " << std::endl;
  try {
    InitSDL();
    CreateWindow(options);
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
