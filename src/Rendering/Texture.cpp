#include <Rendering/Texture.h>
#include <Rendering/Surface.h>
#include <Rendering/Renderer.h>

#include <System/Logger.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture() :
    width(0),
    height(0),
    angle(0.0),
    flip(SDL_FLIP_NONE)
{
}

Texture::Texture(const Texture &tex) :
    width(0),
    height(0),
    angle(0.0),
    flip(SDL_FLIP_NONE)
{
    this->texture = tex.texture;
    this->width = tex.width;
    this->height = tex.height;
}

Texture::~Texture()
{
    Destroy();
}

bool Texture::AllocNewStreaming(int width, int height, int format)
{
    this->width = width;
    this->height = height;
    this->format = format;
    access = SDL_TEXTUREACCESS_STREAMING;
    texture = SDL_CreateTexture(
                  GetRenderer(),
                  format,
                  SDL_TEXTUREACCESS_STREAMING,
                  width, height
              );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create streaming texture: " << SDL_GetError() << std::endl;
        return false;
    }

    /* Enable transparency */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    valid = true;
    return true;
}

bool Texture::AllocFromSurface(Surface &surface)
{
    width = surface.GetWidth();
    height = surface.GetHeight();
    access = SDL_TEXTUREACCESS_STATIC;
    texture = SDL_CreateTextureFromSurface(GetRenderer(), surface.GetSurface());
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture from surface: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    valid = true;
    return true;
}

bool Texture::AllocNewTarget(int width, int height, int format)
{
    this->width = width;
    this->height = height;
    this->format = format;
    access = SDL_TEXTUREACCESS_TARGET;
    texture = SDL_CreateTexture(
                  GetRenderer(),
                  format,
                  SDL_TEXTUREACCESS_TARGET,
                  width, height
              );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create target texture: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    valid = true;
    return true;
}

void Texture::UpdateTexture()
{

}

void Texture::Destroy()
{
    if(!valid) return;
    SDL_DestroyTexture(texture);
    valid = false;
}

void Texture::LockTexture()
{
    if(locked || access != SDL_TEXTUREACCESS_STREAMING) return;
    if(SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch)) {
        Logger::error("RENDERING") << "Unable to lock texture: " << SDL_GetError() << std::endl;
        locked = false;
    }
    else locked = true;
}

void Texture::UnlockTexture()
{
    if(!locked || access != SDL_TEXTUREACCESS_STREAMING) return;
    SDL_UnlockTexture(texture);
    locked = false;
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if(!locked) return;
    uint32_t index = x + y * width;
    pixels[index] = ToPixel(r, g, b, a);
}

void Texture::Rotate(double angle)
{
    this->angle += angle;
}

void Texture::FlipHorizontal()
{
    flip = (SDL_RendererFlip)((int)flip | SDL_FLIP_HORIZONTAL);
}

void Texture::FlipVertical()
{
    flip = (SDL_RendererFlip)((int)flip | SDL_FLIP_VERTICAL);
}

void Texture::FlipNone()
{
    flip = (SDL_RendererFlip)SDL_FLIP_NONE;
}

void Texture::SetAlphaMod(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::Copy(Texture &other, uint32_t x, uint32_t y, SDL_Rect *rect)
{
    if(!locked || !other.IsLocked()) {
        Logger::error("RENDERING") << "Lock the texture before copying from or to it!" << std::endl;
        return;
    }

    uint32_t offX = 0, offY = 0, ow = other.GetWidth(), oh = other.GetHeight();
    if(rect) {
        offX = rect->x;
        offY = rect->y;
        ow = rect->w;
        oh = rect->h;
    }

    if( int(x + ow) > width || int(y + oh) > height ) {
        Logger::error("RENDERING") << "Attempted to copy a texture which is too large for the target (or goes out of bounds)!" << std::endl;
        return;
    }

    for(uint32_t ix = 0; ix < ow; ix++) {
        for(uint32_t iy = 0; iy < oh; iy++) {
            uint32_t index1 = (ix+offX) + (iy+offY) * other.GetWidth();
            uint32_t index2 = (x+ix) + (y+iy) * width;

            Uint8 r, g, b, a;
            FromPixel(other.GetData()[index1], &r, &g, &b, &a);

            if(a != 0) pixels[index2] = ToPixel(r, g, b, a);
        }
    }
}

void Texture::SetBlendMode(SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(texture, mode);
}

Uint32 *Texture::GetData()
{
    if(!locked) return nullptr;
    return pixels;
}

Uint32 Texture::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return r << 24 | g << 16 | b << 8 | a;
}

void Texture::FromPixel(Uint32 value, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    *r = value >> 24;
    *g = (value >> 16) & (uint8_t)0xFF;
    *b = (value >> 8) & (uint16_t)0xFF;
    *a = value & (uint32_t)0xFF;
}
