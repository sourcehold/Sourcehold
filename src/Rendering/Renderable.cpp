#include <Rendering/Renderable.h>

using namespace Sourcehold::Rendering;

Renderable::Renderable()
{
}

Renderable::Renderable(const Renderable& other)
{
    rect = other.rect;
}

Renderable::~Renderable()
{
}

void Renderable::SetRect(SDL_Rect rect)
{
    this->rect = rect;
}


