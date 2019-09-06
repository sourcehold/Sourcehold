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
    this->num = num;

    numRows = (uint32_t)(std::sqrt((float)num) + 0.5f);
    surf.AllocNew(30 * numRows, 16 * numRows);
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
    Texture::Copy(image, coords.first, coords.second);
    image.UnlockTexture();
    Texture::UnlockTexture();
}

SDL_Rect Tileset::GetTile(uint32_t index)
{
    auto coords = IndexToCoords(index);

    SDL_Rect rect;
    rect.x = coords.first;
    rect.y = coords.second;
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
    return std::pair<uint32_t, uint32_t>(30 * (index % numRows), 16 * (index / numRows));
}
