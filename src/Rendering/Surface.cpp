#include <Rendering/Surface.h>
#include <System/Logger.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Surface::Surface(std::shared_ptr<Renderer> rend) : Renderable() {
    this->renderer = rend;
}

Surface::~Surface() {
    Destroy();
}

bool Surface::AllocNew(int width, int height, int format) {
    this->width = width;
    this->height = height;
    surface = SDL_CreateRGBSurface(
        0,
        width,
        height,
        32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32(0xFF000000), // r
        Uint32(0x00FF0000), // g
        Uint32(0x0000FF00), // b
        Uint32(0x000000FF)  // a
#else
        Uint32(0x000000FF), // r
        Uint32(0x0000FF00), // g
        Uint32(0x00FF0000), // b
        Uint32(0xFF000000)  // a
#endif
        );
    if(!surface) {
        Logger::error("RENDERING") << "Unable to create surface: " << SDL_GetError() << std::endl;
        valid = false;
        return false;
    }

    valid = true;
    return true;
}

void Surface::Destroy() {
    SDL_FreeSurface(surface);
    valid = false;
}

void Surface::LockSurface() {
    if(locked) return;
    if(SDL_LockSurface(surface) < 0 ) locked = true;
    else locked = false;
}

void Surface::UnlockSurface() {
    if(!locked) return;
    SDL_UnlockSurface(surface);
}

void Surface::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if(!locked) return;
    Uint32 *target_pixel = (Uint32*)surface->pixels + y * surface->pitch + x * sizeof *target_pixel;
    *target_pixel = ToPixel(r, g, b, a);
}

void Surface::Blit(Surface &other, uint32_t x, uint32_t y, SDL_Rect *rect) {
    if(!locked || !other.IsLocked()) return;

    SDL_Rect dest = {
        (int)x, (int)y, (int)other.GetWidth(), (int)other.GetHeight()
    };

    int err = SDL_BlitSurface(
        other.GetSurface(),
        rect,
        surface,
        &dest
        );
    if(err < 0) {
        Logger::error("RENDERING") << "Unable to blit surface: " << SDL_GetError() << std::endl;
        return;
    }
}

Uint32* Surface::GetData() {
    if(!locked) return nullptr;
    return pixels;
}

Uint32 Surface::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return r << 24 | g << 16 | b << 8 | a;
}

