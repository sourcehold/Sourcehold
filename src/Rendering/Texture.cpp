#include "Rendering/Texture.h"
#include "Rendering/Surface.h"
#include "Rendering/Renderer.h"

#include "System/Logger.h"

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture(const Texture &tex)
    : texture(tex.texture), pixels(tex.pixels) {
}

Texture::~Texture() {
  Destroy();
}

bool Texture::AllocNewStreaming(int width, int height, int format) {
  texture = SDL_CreateTexture(GetRenderer(), format,
                              SDL_TEXTUREACCESS_STREAMING, width, height);
  if (!texture) {
    Logger::error(RENDERING)
        << "Unable to create streaming texture: " << SDL_GetError()
        << std::endl;
    return false;
  }

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

  return true;
}

bool Texture::AllocFromSurface(Surface &surface) {
  texture = SDL_CreateTextureFromSurface(GetRenderer(), surface.GetSurface());
  if (!texture) {
    Logger::error(RENDERING)
        << "Unable to create texture from surface: " << SDL_GetError()
        << std::endl;
    return false;
  }

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  return true;
}

bool Texture::AllocNewTarget(int width, int height, int format) {
  texture = SDL_CreateTexture(GetRenderer(), format, SDL_TEXTUREACCESS_TARGET,
                              width, height);
  if (!texture) {
    Logger::error(RENDERING)
        << "Unable to create target texture: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  return true;
}

void Texture::UpdateTexture() {
}

void Texture::Destroy() {
  if (!texture)
    return;
  SDL_DestroyTexture(texture);
  texture = nullptr;
}

void Texture::LockTexture() {
  int access;
  SDL_QueryTexture(texture, NULL, &access, NULL, NULL);

  if (!texture || access != SDL_TEXTUREACCESS_STREAMING)
    return;
  int pitch;
  if (SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch)) {
    Logger::error(RENDERING)
        << "Unable to lock texture: " << SDL_GetError() << std::endl;
  }
}

void Texture::UnlockTexture() {
  int access;
  SDL_QueryTexture(texture, NULL, &access, NULL, NULL);

  if (access != SDL_TEXTUREACCESS_STREAMING)
    return;
  SDL_UnlockTexture(texture);
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b,
                       uint8_t a) {
  int width = GetWidth();
  uint32_t index = x + y * width;
  pixels[index] = ToPixel(r, g, b, a);
}

void Texture::SetAlphaMod(Uint8 alpha) {
  SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture::Copy(Texture &other, uint32_t x, uint32_t y, SDL_Rect *rect) {
  uint32_t offX = 0, offY = 0, ow = other.GetWidth(), oh = other.GetHeight();
  if (rect) {
    offX = rect->x;
    offY = rect->y;
    ow = rect->w;
    oh = rect->h;
  }

  int width, height;
  GetDim(&width, &height);

  if (int(x + ow) > width || int(y + oh) > height) {
    Logger::error(RENDERING) << "Attempted to copy a texture which is too "
                                "large for the target (or goes out of bounds)!"
                             << std::endl;
    return;
  }

  for (uint32_t ix = 0; ix < ow; ix++) {
    for (uint32_t iy = 0; iy < oh; iy++) {
      uint32_t index1 = (ix + offX) + (iy + offY) * other.GetWidth();
      uint32_t index2 = (x + ix) + (y + iy) * width;

      Uint8 r, g, b, a;
      FromPixel(other.GetData()[index1], &r, &g, &b, &a);

      if (a != 0)
        pixels[index2] = ToPixel(r, g, b, a);
    }
  }
}

void Texture::SetBlendMode(SDL_BlendMode mode) {
  SDL_SetTextureBlendMode(texture, mode);
}

int Texture::GetWidth() {
  int w;
  SDL_QueryTexture(texture, NULL, NULL, &w, NULL);
  return w;
}

int Texture::GetHeight() {
  int h;
  SDL_QueryTexture(texture, NULL, NULL, NULL, &h);
  return h;
}

void Texture::GetDim(int *w, int *h) {
  SDL_QueryTexture(texture, NULL, NULL, w, h);
}

Uint32 *Texture::GetData() {
  return pixels;
}

Uint32 Texture::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  return r << 24 | g << 16 | b << 8 | a;
}

void Texture::FromPixel(Uint32 value, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) {
  *r = value >> 24;
  *g = (value >> 16) & (uint8_t)0xFF;
  *b = (value >> 8) & (uint16_t)0xFF;
  *a = value & (uint32_t)0xFF;
}
