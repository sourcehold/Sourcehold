#pragma once
#include <SDL2/SDL.h>
#include "SDLFactories.h"
namespace Sourcehold {
namespace SDL {

///////////////////////////////////////////////////////////////////////////////
// SDLBackend
// Singleton -- SDLBackend::Instance()
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Initializes the SDL and following SDL objects:
// - SDL_Renderer
// - SDL_Window
// Throws an exception if any fail to construct.
// ----------------------------------------------------------------------------
// Use SDL library functions in conjunction with member SDL objects.
// Use factories to create managed SDL objects.
//
///////////////////////////////////////////////////////////////////////////////
class SDLBackend {
 public:
  static SDLBackend& Instance() {
    static SDLBackend backend_;
    return backend_;
  }

  static SDL_Renderer* Renderer() {
    return Instance().renderer_.get();
  }
  static SDL_Window* Window() {
    return Instance().window_.get();
  }

  SDL_Window_UQ window_;
  SDL_Renderer_UQ renderer_;

 private:
  SDLBackend();
  ~SDLBackend();
};

}  // namespace SDL
}  // namespace Sourcehold
