#include <Texture.h>

using namespace OpenSH::System;
using namespace OpenSH::Rendering;

Texture::Texture() {

}

Texture::Texture(Context &ctx) {
    this->ctx = ctx;
}

Texture::~Texture() {
}

bool Texture::AllocNew(uint32_t width, uint32_t height, uint32_t format) {
    this->width = width;
    this->height = height;
    texture = SDL_CreateTexture(
        ctx.renderer,
        format,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    if(!texture) {
        Logger::error("RENDERING") << "Unable to create texture!" << std::endl;
        return false;
    }

    pixels.resize(width * height * 4, 0);

    return true;
}

void Texture::SetPixel(uint32_t x, uint32_t y) {

}

void Texture::UpdateTexture() {
    SDL_UpdateTexture(
        texture,
        NULL,
        &pixels[0],
        width * 4
    );
}

void Texture::Render(uint32_t x, uint32_t y) {
    SDL_RenderCopy(ctx.renderer, texture, NULL, NULL);
}

void Texture::SetContext(Context &ctx) {
    this->ctx = ctx;
}
