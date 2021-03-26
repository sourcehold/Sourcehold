#include "Rendering/Renderer.h"
#include "Rendering/Display.h"
#include "System/Logger.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace System;

Renderer::Renderer() {
  InitRenderer();
}
Renderer::~Renderer() {
  DestroyRenderer();
}

bool Renderer::InitRenderer() {
  sdl_window_ = GetWindow();
  sdl_renderer_ = SDL_CreateRenderer(
      sdl_window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  if (!sdl_renderer_) {
    Logger::error(RENDERING)
        << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_SetRenderDrawBlendMode(sdl_renderer_, SDL_BLENDMODE_BLEND);

  const char *hint = "SDL_HINT_RENDER_SCALE_QUALITY";
  if (SDL_SetHintWithPriority(hint, "1", SDL_HINT_OVERRIDE) == SDL_FALSE) {
    Logger::warning(RENDERING)
        << "Unable to set " << hint << " hint!" << std::endl;
  }

  return true;
}

void Renderer::DestroyRenderer() {
  if (sdl_renderer_)
    SDL_DestroyRenderer(sdl_renderer_);
}

void Renderer::Update() {
}

void Renderer::Clear() {
  SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sdl_renderer_);
}

void Renderer::Flush() {
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::SetTarget(Texture *target, Rect<int> rect) {
  bounds_ = rect;
  target_ = target;
  auto result = SDL_SetRenderTarget(sdl_renderer_,
                                    target_ ? target_->GetTexture() : nullptr);
  if (result < 0) {
    Logger::error(RENDERING) << "SDL_SetRenderTarget() failed: "  //
                             << SDL_GetError() << std::endl;
  }
}

void Renderer::ResetTarget() {
  target_ = nullptr;
  bounds_ = {0, 0, GetWidth(), GetHeight()};
  SDL_SetRenderTarget(sdl_renderer_, nullptr);
}

void Renderer::Render(Texture &texture, Vector2<int> pos, SDL_Rect *clip) {
  SDL_Rect rect = {pos.x, pos.y, texture.GetWidth(), texture.GetHeight()};

  if (clip) {
    rect.w = clip->w;
    rect.h = clip->h;
  }

  SDL_RenderCopyEx(sdl_renderer_, texture.GetTexture(), clip, &rect, 0.0,
                   nullptr, SDL_FLIP_NONE);
}

void Renderer::Render(Texture &texture, Rect<int> space, SDL_Rect *clip) {
  // Source texture, specified by SDL_Rect, will
  // be stretched to fit the destination rect
  SDL_Rect rect = {space.x, space.y, space.w, space.h};

  SDL_RenderCopyEx(sdl_renderer_, texture.GetTexture(), clip, &rect, 0.0,
                   nullptr, SDL_FLIP_NONE);
}

void Renderer::Render(Texture &texture, SDL_Rect *clip) {
  SDL_RenderCopyEx(sdl_renderer_, texture.GetTexture(), clip, nullptr, 0.0,
                   nullptr, SDL_FLIP_NONE);
}

void Renderer::Fill(Color color) {
  SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(sdl_renderer_, nullptr);
}

void Renderer::RenderRect(Rect<int> rect, Color color, bool solid) {
  SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, color.a);

  SDL_Rect rc = {rect.x, rect.y, rect.w, rect.h};

  if (solid) {
    SDL_RenderFillRect(sdl_renderer_, &rc);
  }
  else {
    SDL_RenderDrawRect(sdl_renderer_, &rc);
  }
}

void Renderer::RenderLine(Line<int> line, Color color) {
  SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.b, color.g,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(sdl_renderer_,        //
                     bounds_.x + line.x1,  //
                     bounds_.y + line.y1,  //
                     bounds_.x + line.x2,  //
                     bounds_.y + line.y2);
}

SDL_BlendMode Renderer::GetAlphaKeyBlendMode() {
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
