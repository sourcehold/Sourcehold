#include <Rendering/Texture.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture() {

}

Texture::Texture(Context &ctx) {
    this->ctx = ctx;
}

Texture::~Texture() {
}

bool Texture::AllocNew(int width, int height, int format) {
    this->width = width;
    this->height = height;
    texture = SDL_CreateTexture(
        ctx.renderer,
        format,
        SDL_TEXTUREACCESS_STATIC,
        width, height
    );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture!" << std::endl;
        return false;
    }

    pixels.resize(width * height, 0);

    return true;
}

Uint32 color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   return r << 24 | g << 16 | b << 8 | a;
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    pixels[x + y * width] = color(r, g, b, 0xFF);
}

void Texture::UpdateTexture() {
    SDL_UpdateTexture(
        texture,
        NULL,
        &pixels[0],
        width * 4
    );
}

void Texture::SetContext(Context &ctx) {
    this->ctx = ctx;
}

SDL_Texture *Texture::GetTexture() {
    return texture;
}

int Texture::GetWidth() {
    return width;
}

int Texture::GetHeight() {
    return height;
}
