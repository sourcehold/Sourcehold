#include <Rendering/Texture.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture(SDL_Renderer* rend) :
    angle(0.0),
    width(0),
    height(0),
    flip(SDL_FLIP_NONE)
{
    this->renderer = rend;
}

Texture::Texture(const Texture &tex) :
    pixels(tex.pixels),
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
}

bool Texture::AllocNew(int width, int height, int format) {
    this->width = width;
    this->height = height;
    texture = SDL_CreateTexture(
        renderer,
        format,
        SDL_TEXTUREACCESS_STATIC,
        width, height
    );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    pixels.resize(width * height, 0);

    /* Enable transparency */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    /* Clear texture */
    for(uint32_t i = 0; i < pixels.size(); i++) {
        pixels[i] = 0x00;
    }

    return true;
}

void Texture::UpdateTexture() {
    SDL_UpdateTexture(
        texture,
        NULL,
        &pixels[0],
        width * 4
    );
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    uint32_t index = x + y * width;
    if(index >= pixels.size()) return;
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

Uint32 Texture::ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   return r << 24 | g << 16 | b << 8 | a;
}
