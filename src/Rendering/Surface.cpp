#include "Rendering/Surface.h"
#include "System/Logger.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Surface::Surface(Vector2<int> size) {
  surface_ = SDL::MakeSurfaceUQ(0, size.x, size.y, SDL::SurfaceDepth,  //
                                SDL::SurfaceMask.r,                    //
                                SDL::SurfaceMask.g,                    //
                                SDL::SurfaceMask.b,                    //
                                SDL::SurfaceMask.a);
  if (!surface_) {
    auto err = "Unable to create surface: " + std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetSurfaceBlendMode(surface_.get(), SDL_BLENDMODE_BLEND);
}

void Surface::Lock() const noexcept {
  SDL_LockSurface(surface_.get());
}

void Surface::Unlock() const noexcept {
  SDL_UnlockSurface(surface_.get());
}

void Surface::Set(Vector2<int> pos, Color color) const noexcept {
  SDL::At<uint32_t>(surface_.get(), pos) = AsPixel(color);
}
void Surface::Blit(const Surface& other, Vector2<int> pos,
                   std::optional<Rect<int>> clip) const noexcept {
  SDL_Rect dest = {pos.x, pos.y, other.surface_->w, other.surface_->h};
  SDL_Rect* clip_ = clip.has_value() ? SDL::ToSDLRectPtr(clip.value())  //
                                     : nullptr;

  int err = SDL_BlitSurface(other.surface_.get(), clip_, surface_.get(), &dest);
  if (err < 0) {
    Logger::error(RENDERING)
        << "Unable to blit surface: " << SDL_GetError() << std::endl;
  }
}

void Surface::Fill(Color color) const noexcept {
  SDL_FillRect(surface_.get(), nullptr,
               SDL_MapRGBA(surface_->format,  //
                           color.r, color.g, color.b, color.a));
}
