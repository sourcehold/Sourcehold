#pragma once
#include <SDL2/SDL.h>
#include "SDLFactories.h"
namespace Sourcehold {
namespace SDL {

///////////////////////////////////////////////////////////////////////////////
// SDLBackend
// Singleton -- SDLBackend::Instance()
// ----------------------------------------------------------------------------
//
// Synopsis
//
// ----------------------------------------------------------------------------
// Initializes the SDL and common SDL objects:
// - SDL_Renderer
// - SDL_Window
// Throws an exception if any fails to construct.
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

  SDL_Window_UQ window_;
  SDL_Renderer_UQ renderer_;

 private:
  SDLBackend();
  ~SDLBackend();
};

}  // namespace SDL
}  // namespace Sourcehold
