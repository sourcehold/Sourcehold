#include <Rendering/Texture.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture(std::shared_ptr<Renderer> rend) :
    angle(0.0),
    width(0),
    height(0),
    flip(SDL_FLIP_NONE),
    renderer(rend)
{
}

Texture::Texture(const Texture &tex) :
    angle(0.0),
    width(0),
    height(0),
    flip(SDL_FLIP_NONE)
{
    this->renderer = tex.renderer;
    this->texture = tex.texture;
    this->width = tex.width;
    this->height = tex.height;
}

Texture::~Texture() {
    Destroy();
}

bool Texture::AllocNew(int width, int height, int format) {
    this->width = width;
    this->height = height;
    texture = SDL_CreateTexture(
        renderer->GetRenderer(),
        format,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    /* Enable transparency */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return true;
}

void Texture::UpdateTexture() {

}

void Texture::Destroy() {
    if(texture) SDL_DestroyTexture(texture);
}

void Texture::LockTexture() {
    if(locked) return;
    if(SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch)) {
        Logger::error("RENDERING") << "Unable to lock texture: " << SDL_GetError() << std::endl;
        locked = false;
    }else locked = true;
}

void Texture::UnlockTexture() {
    if(!locked) return;
    SDL_UnlockTexture(texture);
    locked = false;
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if(!locked) return;
    uint32_t index = x + y * width;
    pixels[index] = ToPixel(r, g, b, a);
}

void Texture::Rotate(double angle) {
    this->angle += angle;
}

void Texture::FlipHorizontal() {
    flip = (SDL_RendererFlip)((int)flip | SDL_FLIP_HORIZONTAL);
}

void Texture::FlipVertical() {
    flip = (SDL_RendererFlip)((int)flip | SDL_FLIP_VERTICAL);
}

void Texture::FlipNone() {
    flip = (SDL_RendererFlip)SDL_FLIP_NONE;
}

void Texture::SetAlphaMod(Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::Copy(Texture &other, uint32_t x, uint32_t y) {
    if(!locked || !other.IsLocked() || x > width || y > height) return;
    if( other.GetWidth() > width ||
        other.GetHeight() > height ||
        x + other.GetWidth() > width ||
        y + other.GetHeight() > height
    ) {
        Logger::error("RENDERING") << "Attempted to copy a texture which is too large for the target (or goes out of bounds)!" << std::endl;
        return;
    }

    for(uint32_t ix = 0; ix < other.GetWidth(); ix++) {
        for(uint32_t iy = 0; iy < other.GetHeight(); iy++) {
            uint32_t index = ix + iy * other.GetWidth();
            Uint32 value = other.GetData()[index];
            pixels[(x+ix) + (y+iy) * width] = value;
        }
    }
}

uint32_t *Texture::GetData() {
    if(!locked) return nullptr;
    return pixels;
}

Uint32 Texture::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   return r << 24 | g << 16 | b << 8 | a;
}
