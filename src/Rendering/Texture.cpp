#include <Rendering/Texture.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Texture::Texture() {

}

Texture::~Texture() {
}

bool Texture::AllocNew(int width, int height, int format) {
    /*this->width = width;
    this->height = height;
    texture = SDL_CreateTexture(
        ctx.renderer,
        format,
        SDL_TEXTUREACCESS_STATIC,
        width, height
    );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture: " << SDL_GetError() << std::endl;
        return false;
    }*/

    pixels.resize(width * height, 0);

    /* Enable transparency */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    /* Clear texture */
    for(uint32_t i = 0; i < pixels.size(); i++) {
        pixels[i] = 0x00;
    }

    return true;
}

Uint32 color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   return r << 24 | g << 16 | b << 8 | a;
}

void Texture::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    pixels[x + y * width] = color(r, g, b, a);
}

void Texture::UpdateTexture() {
    SDL_UpdateTexture(
        texture,
        NULL,
        &pixels[0],
        width * 4
    );
}

SDL_Texture *Texture::GetTexture() {
    return texture;
}

uint32_t *Texture::GetData() {
    return pixels.data();
}

int Texture::GetWidth() {
    return width;
}

int Texture::GetHeight() {
    return height;
}
