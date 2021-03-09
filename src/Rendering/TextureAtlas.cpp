#include "Rendering/TextureAtlas.h"

#include <cmath>
#include <algorithm>

using namespace Sourcehold::Rendering;

TextureAtlas::TextureAtlas(const TextureAtlas &atlas) :
    num_(atlas.num_),
    entries_(atlas.entries_),
    surf_(atlas.surf_)
{
}

TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::Allocate(std::vector<std::pair<uint32_t, uint32_t>>& entries)
{
    if(!entries.size()) {
        return;
    }

    struct Elem {
        std::pair<uint32_t, uint32_t> dim;
        size_t index;
    };

    std::vector<Elem> sorted(entries.size());
    for(size_t i = 0; i < sorted.size(); i++) {
        Elem elem;
        elem.index = i;
        elem.dim = entries[i];
        sorted[i] = elem;
    }

    std::sort(
        sorted.begin(),
        sorted.end(),
    [](Elem& e1, Elem& e2) -> bool {
        return e1.dim.first > e2.dim.first;
    }
    );

    this->entries_.resize(entries.size());

    uint32_t width = 0, height = 0;
    uint32_t px = 0, py = 0;
    for(size_t i = 0; i < sorted.size(); i++) {
        Elem e = sorted[i];

        if(px > max_x_resolution_) {
            px = 0;
            py = height;
        }

        SDL_Rect rect;
        rect.x = static_cast<int>(px);
        rect.y = static_cast<int>(py);
        rect.w = static_cast<int>(e.dim.first);
        rect.h = static_cast<int>(e.dim.second);

        px += e.dim.first;

        width = std::max(width, px);
        height = std::max(height, py + e.dim.second);

        this->entries_[e.index] = rect;
    }

    surf_.AllocNew(static_cast<int>(width), static_cast<int>(height));
}

void TextureAtlas::Create()
{
    Texture::AllocFromSurface(surf_);
    surf_.Destroy();
}

SDL_Rect TextureAtlas::Get(uint32_t index)
{
    if(index < entries_.size()) return entries_[index];
    else return { 0, 0, 0, 0 };
}

void TextureAtlas::Clear()
{
    Texture::Destroy();
}

void TextureAtlas::Lock()
{
    surf_.LockSurface();
}

void TextureAtlas::Unlock()
{
    surf_.UnlockSurface();
}

Uint32 *TextureAtlas::GetData()
{
    return surf_.GetData();
}

// TODO:
// Missing implementation
std::pair<uint32_t, uint32_t> TextureAtlas::IndexToCoords([[maybe_unused]] uint32_t index)
{
    return std::pair<uint32_t, uint32_t>(0, 0);
}
