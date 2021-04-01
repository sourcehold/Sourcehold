#include "Rendering/Renderer.h"
#include "Rendering/Display.h"
#include "SDL/SDLBackend.h"
#include "System/Logger.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace System;
using namespace SDL;

static Texture *_target = nullptr;
static Rect<int> _tr;

void Rendering::ClearDisplay() {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(SDLBackend::Renderer());
}

void Rendering::FlushDisplay() {
  SDL_RenderPresent(SDLBackend::Renderer());
}

void Rendering::SetTarget(Texture *target, Rect<int> rect) {
  _tr = rect;
  _target = target;
  if (SDL_SetRenderTarget(SDLBackend::Renderer(),
                          target ? target->GetTexture() : nullptr) < 0) {
#if 0
        Logger::error(RENDERING) << "SDL_SetRenderTarget() failed: " << SDL_GetError() << std::endl;
#endif
  }
}
void Rendering::ResetTarget() {
  auto display_size = Display::Size();
  _target = nullptr;
  _tr = {0, 0, display_size.x, display_size.y};
  SDL_SetRenderTarget(SDL::SDLBackend::Instance().renderer_.get(), nullptr);
}

void Rendering::Render(Texture &texture, int x, int y, SDL_Rect *clip) {
  SDL_Rect rect = {x, y, texture.GetWidth(), texture.GetHeight()};

  if (clip) {
    rect.w = clip->w;
    rect.h = clip->h;
  }

  SDL_RenderCopyEx(SDLBackend::Renderer(), texture.GetTexture(), clip, &rect,
                   0.0, nullptr, SDL_FLIP_NONE);
}

void Rendering::Render(Texture &texture, int x, int y, int w, int h,
                       SDL_Rect *clip) {
  /**
   * Source texture, specified by SDL_Rect, will
   * be stretched to fit the destination rect
   */
  SDL_Rect rect = {x, y, w, h};

  SDL_RenderCopyEx(SDLBackend::Renderer(), texture.GetTexture(), clip, &rect,
                   0.0, nullptr, SDL_FLIP_NONE);
}

void Rendering::Render(Texture &texture, SDL_Rect *clip) {
  SDL_RenderCopyEx(SDLBackend::Renderer(), texture.GetTexture(), clip, nullptr,
                   0.0, nullptr, SDL_FLIP_NONE);
}

void Rendering::Fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(), r, g, b, a);
  SDL_RenderFillRect(SDLBackend::Renderer(), nullptr);
}

void Rendering::RenderRect(Rect<int> rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                           bool solid) {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(), r, g, b, a);

  SDL_Rect rc = {rect.x, rect.y, rect.w, rect.h};

  if (solid) {
    SDL_RenderFillRect(SDLBackend::Renderer(), &rc);
  }
  else {
    SDL_RenderDrawRect(SDLBackend::Renderer(), &rc);
  }
}

void Rendering::RenderLine(Line<int> line, Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(), r, b, g, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(SDLBackend::Renderer(), _tr.x + line.x1, _tr.y + line.y1,
                     _tr.x + line.x2, _tr.y + line.y2);
}

Rect<int> Rendering::GetTarget() {
  return _tr;
}
