#include "Rendering/Renderer.h"
#include "Rendering/Display.h"
#include "System/Logger.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace System;

static SDL_Renderer *_renderer;
static SDL_Window *_window;
static Texture *_target = nullptr;
static Rect<int> _tr;

bool Rendering::InitRenderer() {
  _window = GetWindow();
  _renderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  if (!_renderer) {
    Logger::error(RENDERING)
        << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

  const char *hint = "SDL_HINT_RENDER_SCALE_QUALITY";
  if (SDL_SetHintWithPriority(hint, "1", SDL_HINT_OVERRIDE) == SDL_FALSE) {
    Logger::warning(RENDERING)
        << "Unable to set " << hint << " hint!" << std::endl;
  }

  return true;
}

void Rendering::DestroyRenderer() {
  if (_renderer)
    SDL_DestroyRenderer(_renderer);
}

void Rendering::UpdateRenderer() {
}

void Rendering::ClearDisplay() {
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(_renderer);
}

void Rendering::FlushDisplay() {
  SDL_RenderPresent(_renderer);
}

void Rendering::SetTarget(Texture *target, Rect<int> rect) {
  _tr = rect;
  _target = target;
  if (SDL_SetRenderTarget(_renderer, target ? target->GetTexture() : nullptr) <
      0) {
#if 0
        Logger::error(RENDERING) << "SDL_SetRenderTarget() failed: " << SDL_GetError() << std::endl;
#endif
  }
}

void Rendering::ResetTarget() {
  _target = nullptr;
  _tr = {0, 0, GetWidth(), GetHeight()};
  SDL_SetRenderTarget(_renderer, nullptr);
}

void Rendering::Render(Texture &texture, int x, int y, SDL_Rect *clip) {
  SDL_Rect rect = {x, y, texture.GetWidth(), texture.GetHeight()};

  if (clip) {
    rect.w = clip->w;
    rect.h = clip->h;
  }

  SDL_RenderCopyEx(_renderer, texture.GetTexture(), clip, &rect, 0.0, nullptr,
                   SDL_FLIP_NONE);
}

void Rendering::Render(Texture &texture, int x, int y, int w, int h,
                       SDL_Rect *clip) {
  /**
   * Source texture, specified by SDL_Rect, will
   * be stretched to fit the destination rect
   */
  SDL_Rect rect = {x, y, w, h};

  SDL_RenderCopyEx(_renderer, texture.GetTexture(), clip, &rect, 0.0, nullptr,
                   SDL_FLIP_NONE);
}

void Rendering::Render(Texture &texture, SDL_Rect *clip) {
  SDL_RenderCopyEx(_renderer, texture.GetTexture(), clip, nullptr, 0.0, nullptr,
                   SDL_FLIP_NONE);
}

void Rendering::Fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
  SDL_RenderFillRect(_renderer, nullptr);
}

void Rendering::RenderRect(Rect<int> rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                           bool solid) {
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);

  SDL_Rect rc = {rect.x, rect.y, rect.w, rect.h};

  if (solid) {
    SDL_RenderFillRect(_renderer, &rc);
  } else {
    SDL_RenderDrawRect(_renderer, &rc);
  }
}

void Rendering::RenderLine(Line<int> line, Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetRenderDrawColor(_renderer, r, b, g, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(_renderer, _tr.x + line.x1, _tr.y + line.y1,
                     _tr.x + line.x2, _tr.y + line.y2);
}

SDL_BlendMode Rendering::GetAlphaKeyBlendMode() {
#if 0
    return SDL_ComposeCustomBlendMode(
               SDL_BLENDFACTOR_ONE,
               SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
               SDL_BLENDOPERATION_ADD,
               SDL_BLENDFACTOR_SRC_ALPHA,
               SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
               SDL_BLENDOPERATION_SUBTRACT
           );
#else
  return SDL_BLENDMODE_BLEND;
#endif
}

Rect<int> Rendering::GetTarget() {
  return _tr;
}

SDL_Renderer *Rendering::GetRenderer() {
  return _renderer;
}
