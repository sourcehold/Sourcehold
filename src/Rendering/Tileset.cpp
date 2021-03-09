#include "Rendering/Tileset.h"
#include "System/Logger.h"

#include <cmath>
#include <utility>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Tileset::Tileset()
{
}

Tileset::~Tileset()
{

}

void Tileset::Allocate(uint32_t num)
{
    this->num_ = num;

    numRows_ = static_cast<uint32_t>(std::sqrt(static_cast<float>(num)) + 0.5f);
    surf.AllocNew(static_cast<int>(30 * numRows_), static_cast<int>(16 * numRows_));
}

void Tileset::Create()
{
    Texture::AllocFromSurface(surf);
    surf.Destroy();
}

void Tileset::SetTile(Texture &image, uint32_t index)
{
    auto coords = IndexToCoords(index);

    image.LockTexture();
    Texture::LockTexture();
    Texture::Copy(image, static_cast<int>(coords.first), static_cast<int>(coords.second));
    image.UnlockTexture();
    Texture::UnlockTexture();
}

SDL_Rect Tileset::GetTile(uint32_t index)
{
    auto coords = IndexToCoords(index);

    SDL_Rect rect;
    rect.x = static_cast<int>(coords.first);
    rect.y = static_cast<int>(coords.second);
    rect.w = 30;
    rect.h = 16;

    return rect;
}

void Tileset::Clear()
{
    Texture::Destroy();
}

Uint32 *Tileset::GetData()
{
    return surf.GetData();
}

void Tileset::Lock()
{
    surf.LockSurface();
}

void Tileset::Unlock()
{
    surf.UnlockSurface();
}

std::pair<uint32_t, uint32_t> Tileset::IndexToCoords(uint32_t index)
{
    return { 30 * (index % numRows_), 16 * (index / numRows_) };
}
