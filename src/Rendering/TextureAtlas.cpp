#include <Rendering/TextureAtlas.h>

#include <cmath>
#include <algorithm>
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

void TextureAtlas::Allocate(std::vector<uint32_t> entries) {
    if(!entries.size()) {
        return;   
    }

    /* Sort by amount of pixels */
    std::sort(
        entries.begin(),
        entries.begin() + entries.size(), 
        [](uint32_t e1, uint32_t e2) -> bool {
            return 
                (UnpackWidth(e1) * UnpackHeight(e1)) <
                (UnpackWidth(e2) * UnpackHeight(e2));
        }
    );

    for(uint32_t i = 0; i < entries.size(); i++) {
        uint32_t e = entries[i];

        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = UnpackWidth(e);
        rect.h = UnpackHeight(e);

        this->entries.push_back(rect);
    }

    Texture::AllocNew(800, 600, SDL_PIXELFORMAT_RGBA8888);
}

void TextureAtlas::Allocate(uint32_t num, uint16_t width, uint16_t height) {
    this->num = num;
    Texture::AllocNew(width, height, SDL_PIXELFORMAT_RGBA8888);
    entries.resize(num);
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
    if(index >= entries.size()) return { 0, 0, 10, 10 };
    return entries[index];
}

void TextureAtlas::Clear() {
    Texture::Destroy();
}

std::pair<uint32_t, uint32_t> TextureAtlas::IndexToCoords(uint32_t index) {
    return std::pair<uint32_t, uint32_t>(0, 0);
}
