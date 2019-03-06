#include <Rendering/Tileset.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Tileset::Tileset(std::shared_ptr<Renderer> rend) :
    renderer(rend),
    Texture(rend)
{
}

Tileset::~Tileset()  {

}

void Tileset::Allocate(uint32_t numRows) {
    width = 30 * numRows;
    height = 16 * numRows;
    Texture::AllocNew(width, height, SDL_PIXELFORMAT_RGBA8888);
}

void Tileset::SetEntry(Texture &image, uint32_t x, uint32_t y) {
    if(x > width || y > height) return;

    image.LockTexture();
    Texture::LockTexture();
    Texture::Copy(image, (x) * 30, (y) * 16);
    image.UnlockTexture();
    Texture::UnlockTexture();
}
