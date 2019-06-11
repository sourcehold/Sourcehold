#include <Building.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

Building::Building(uint32_t mw, uint32_t mh)
{
    mapW = mw;
    mapH = mh;
}

Building::Building(std::weak_ptr<Gm1File> file, uint32_t x, uint32_t y, uint32_t mw, uint32_t mh)
{
    gm1 = file.lock();
    mapW = mw;
    mapH = mh;
    PlaceAt(x, y);
}

Building::~Building()
{
}

void Building::Preview(uint32_t x, uint32_t y)
{

}

void Building::PlaceAt(uint32_t x, uint32_t y)
{
    px = x;
    py = y;
    placed = true;
}

void Building::MoveTo(uint32_t x, uint32_t y)
{
    px = x;
    py = y;
}

void Building::Unload()
{
    gm1.reset();
    loaded = false;
}

void Building::Render()
{

}

bool Building::CanWalkOn(uint32_t x, uint32_t y)
{
    uint32_t index = CoordToLocalIndex(x, y);
    for(uint32_t tile : walkableTiles) {
        if(tile == index) return true;
    }
    return false;
}

uint32_t Building::CoordToGlobalIndex(uint32_t x, uint32_t y)
{
    return 0;
}

uint32_t Building::CoordToLocalIndex(uint32_t x, uint32_t y)
{
    return 0;
}

