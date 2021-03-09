#include "Rendering/Texture.h"
#include "Rendering/Surface.h"
#include "Rendering/Renderer.h"

#include "System/Logger.h"

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture(const Texture &tex) :
    texture_(tex.texture_),
    pixels_(tex.pixels_)
{
}

Texture::~Texture()
{
    Destroy();
}

bool Texture::AllocNewStreaming(int width, int height, Uint32 format)
{
    texture_ = SDL_CreateTexture(
                  GetRenderer(),
                  format,
                  SDL_TEXTUREACCESS_STREAMING,
                  width, height
              );
    if(!texture_) {
        Logger::error(RENDERING) << "Unable to create streaming texture: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);

    return true;
}

bool Texture::AllocFromSurface(Surface &surface)
{
    texture_ = SDL_CreateTextureFromSurface(GetRenderer(), surface.GetSurface());
    if(!texture_) {
        Logger::error(RENDERING) << "Unable to create texture from surface: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
    return true;
}

// TODO: DRY
bool Texture::AllocNewTarget(int width, int height, Uint32 format)
{
    texture_ = SDL_CreateTexture(
                  GetRenderer(),
                  format,
                  SDL_TEXTUREACCESS_TARGET,
                  width, height
              );
    if(!texture_) {
        Logger::error(RENDERING) << "Unable to create target texture: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
    return true;
}

void Texture::UpdateTexture()
{

}

void Texture::Destroy()
{
    if (!texture_) return;
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
}

void Texture::LockTexture()
{
    int access;
    SDL_QueryTexture(texture_, NULL, &access, NULL, NULL);

    if(!texture_ || access != SDL_TEXTUREACCESS_STREAMING) return;
    int pitch;
    if(SDL_LockTexture(texture_, nullptr, reinterpret_cast<void**>(&pixels_), &pitch)) {
        Logger::error(RENDERING) << "Unable to lock texture: " << SDL_GetError() << std::endl;
    }
}

void Texture::UnlockTexture()
{
    int access;
    SDL_QueryTexture(texture_, NULL, &access, NULL, NULL);

    if(access != SDL_TEXTUREACCESS_STREAMING) return;
    SDL_UnlockTexture(texture_);
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int width = GetWidth();
    uint32_t index = static_cast<uint32_t>(x + y * width);
    pixels_[index] = ToPixel(r, g, b, a);
}

void Texture::SetAlphaMod(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture_, alpha);
}

void Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(texture_, r, g, b);
}

void Texture::Copy(Texture &other, int32_t x, int32_t y, SDL_Rect *rect)
{
    int32_t offX = 0, offY = 0, ow = other.GetWidth(), oh = other.GetHeight();
    if(rect) {
        offX = rect->x;
        offY = rect->y;
        ow = rect->w;
        oh = rect->h;
    }

    int width, height;
    GetDim(&width, &height);

    if( int(x + ow) > width || int(y + oh) > height ) {
        Logger::error(RENDERING) << "Attempted to copy a texture which is too large for the target (or goes out of bounds)!" << std::endl;
        return;
    }

    for(auto ix = 0; ix < ow; ++ix) {
        for(auto iy = 0; iy < oh; iy++) {
            auto index1 = (ix+offX) + (iy+offY) * other.GetWidth();
            auto index2 = (x+ix) + (y+iy) * width;

            Uint8 r, g, b, a;
            FromPixel(other.GetData()[index1], &r, &g, &b, &a);

            if(a != 0) pixels_[index2] = ToPixel(r, g, b, a);
        }
    }
}

void Texture::SetBlendMode(SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(texture_, mode);
}

int Texture::GetWidth()
{
    int w;
    SDL_QueryTexture(
        texture_,
        NULL,
        NULL,
        &w,
        NULL
    );
    return w;
}

int Texture::GetHeight()
{
    int h;
    SDL_QueryTexture(
        texture_,
        NULL,
        NULL,
        NULL,
        &h
    );
    return h;
}

void Texture::GetDim(int* w, int* h)
{
    SDL_QueryTexture(
        texture_,
        NULL,
        NULL,
        w,
        h
    );
}

Uint32 *Texture::GetData()
{
    return pixels_;
}

Uint32 Texture::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  // TODO: DRY
    return static_cast<Uint32>(r << 24 | g << 16 | b << 8 | a);
}

void Texture::FromPixel(Uint32 value, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    *r = value >> 24;
    *g = (value >> 16) & 0xFF;
    *b = (value >> 8) & 0xFF;
    *a = value & 0xFF;
}
