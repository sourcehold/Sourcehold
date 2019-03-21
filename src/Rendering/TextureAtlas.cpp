#include <Rendering/TextureAtlas.h>
#include <cmath>
#include <utility>

using namespace Sourcehold::Rendering;

TextureAtlas::TextureAtlas(std::shared_ptr<Renderer> rend) :
    Texture(rend),
    renderer(rend)
{
}

TextureAtlas::TextureAtlas(const TextureAtlas &atlas) : Texture(atlas.renderer)
{
    this->renderer = atlas.renderer;
}

TextureAtlas::~TextureAtlas() {
    Clear();
}

void TextureAtlas::Allocate(uint32_t num, uint16_t width, uint16_t height) {
    this->numRows = (uint32_t)(std::sqrt((float)num) + 0.5f);
    this->num = num;
    this->nx = width;
    this->ny = height;
    Texture::AllocNew(width * numRows, height * numRows, SDL_PIXELFORMAT_RGBA8888);
}

void TextureAtlas::Set(Texture &image, uint32_t index) {
    auto coords = IndexToCoords(index);

    image.LockTexture();
    Texture::LockTexture();
    Texture::Copy(image, coords.first, coords.second);
    image.UnlockTexture();
    Texture::UnlockTexture();
}

SDL_Rect TextureAtlas::Get(uint32_t index) {
    auto coords = IndexToCoords(index);

    SDL_Rect rect;
    rect.x = coords.first;
    rect.y = coords.second;
    rect.w = nx;
    rect.h = ny;

    return rect;
}

void TextureAtlas::Clear() {
    Texture::Destroy();
}

std::pair<uint32_t, uint32_t> TextureAtlas::IndexToCoords(uint32_t index) {
    return std::pair<uint32_t, uint32_t>(nx * (index % numRows), ny * (index / numRows));
}
