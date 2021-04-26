#pragma once
#include <SDL2/SDL_surface.h>
namespace Sourcehold {
namespace SDL {
struct SurfaceScopedLock {
  SurfaceScopedLock(SDL_Surface* surface_to_lock) : surf(surface_to_lock) {
    SDL_LockSurface(surf);
  }
  ~SurfaceScopedLock() {
    SDL_UnlockSurface(surf);
  }
  SDL_Surface* surf;
};
}  // namespace SDL
}  // namespace Sourcehold
