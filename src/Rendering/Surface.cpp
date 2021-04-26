#include "Rendering/Surface.h"
#include "System/Logger.h"
#include "SDL/SDLFactories.h"
#include "SDL/SurfaceLock.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Surface::Surface(const Surface& other) {
  if (other.surface_) {
    auto lock = SDL::SurfaceScopedLock(other);

    surface_ = SDL::MakeSurfaceUQ(0, other.surface_->w, other.surface_->h,
                                  Depth, Mask.r, Mask.g, Mask.b, Mask.a);

    std::copy(std::begin(other), std::end(other), this->begin());
  }
  else {
    surface_ = nullptr;
  }
}

Surface::Surface(Surface&& other) {
  surface_ = std::move(other.surface_);
}

Surface& Surface::operator=(const Surface& other) {
  if (other.surface_) {
    auto lock = SDL::SurfaceScopedLock(other);

    surface_ = SDL::MakeSurfaceUQ(0, other.surface_->w, other.surface_->h,
                                  Depth, Mask.r, Mask.g, Mask.b, Mask.a);

    std::copy(std::begin(other), std::end(other), this->begin());
  }
  else {
    surface_ = nullptr;
  }

  return *this;
}

Surface& Surface::operator=(Surface&& other) {
  surface_ = std::move(other.surface_);
  return *this;
}

Surface::Surface(Vector2<int> size) {
  surface_ = SDL::MakeSurfaceUQ(0, size.x, size.y, Depth,  //
                                Mask.r,                    //
                                Mask.g,                    //
                                Mask.b,                    //
                                Mask.a);
  if (!surface_) {
    auto err = "Unable to create surface: " + std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetSurfaceBlendMode(surface_.get(), BlendMode);
}

void Surface::Set(Vector2<int> pos, Color color) const noexcept {
  SDL::At<Pixel>(surface_.get(), pos) = static_cast<Pixel>(color);
}
void Surface::Blit(const Surface& other, Vector2<int> pos,
                   std::optional<Rect<int>> clip) const noexcept {
  SDL_Rect dest = {pos.x, pos.y, other.surface_->w, other.surface_->h};
  SDL_Rect* clip_ = clip.has_value() ? SDL::ToSDLRectPtr(*clip)  //
                                     : nullptr;

  int err = SDL_BlitSurface(other, clip_, surface_.get(), &dest);
  if (err < 0) {
    Logger::error(RENDERING)
        << "Unable to blit surface: " << SDL_GetError() << std::endl;
  }
}

void Surface::Fill(Color color) const noexcept {
  SDL_FillRect(surface_.get(), nullptr, color);
}

Pixel* Surface::begin() const noexcept {
  return static_cast<Pixel*>(surface_->pixels);
}
Pixel* Surface::end() const noexcept {
  return &static_cast<Pixel*>(surface_->pixels)[surface_->w * surface_->h];
}
const Pixel* Surface::cbegin() const noexcept {
  return begin();
}
const Pixel* Surface::cend() const noexcept {
  return end();
}
