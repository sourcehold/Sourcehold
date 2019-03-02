#include <Rendering/TextureAtlas.h>

using namespace Sourcehold::Rendering;

TextureAtlas::TextureAtlas(std::shared_ptr<Renderer> rend, uint32_t hint) :
    std::vector<Texture>(),
    renderer(rend)
{
    vector::reserve(hint);
}

TextureAtlas::TextureAtlas(const TextureAtlas &atlas) :
    std::vector<Texture>(atlas)
{
    this->renderer = atlas.renderer;
}

TextureAtlas::~TextureAtlas() {
    vector::clear();
}

void TextureAtlas::Add(Texture &tex) {
    vector::push_back(tex);
}

void TextureAtlas::Set(Texture &tex, uint32_t index) {
    vector::at(index) = tex;
}

Texture &TextureAtlas::Get(uint32_t index) {
    return vector::at(index);
}

void TextureAtlas::Remove(uint32_t index) {
    vector::erase(vector::begin() + index);
}

void TextureAtlas::Resize(uint32_t num) {
    vector::resize(num, renderer);
}

void TextureAtlas::Clear() {
    vector::clear();
}
