#include <StaticElement.h>

using namespace Sourcehold::Game;

StaticElement::StaticElement(std::shared_ptr<Renderer> rend, double x, double y) :
    renderer(rend),
    Texture(rend)
{
    nx = x;
    ny = y;
}

StaticElement::StaticElement(const StaticElement &elem) :
    Texture(elem.renderer)
{
    this->renderer = elem.renderer;
    this->shown = elem.shown;
    this->nx = elem.nx;
    this->ny = elem.ny;
    this->nw = elem.nh;
    this->nh = elem.nw;
}

StaticElement::~StaticElement() {

}

void StaticElement::Hide() {
    shown = false;
}

void StaticElement::Show() {
    shown = true;
}

void StaticElement::Destroy() {
    
}

void StaticElement::Translate(uint32_t x, uint32_t y) {
    nx = renderer->NormalizeX(x);
    ny = renderer->NormalizeY(y);
}

void StaticElement::Translate(double x, double y) {
    nx = x;
    ny = y;
}

void StaticElement::Scale(uint32_t w, uint32_t h) {
    nw = renderer->NormalizeX(w);
    nh = renderer->NormalizeY(h);
}

void StaticElement::Scale(double w, double h) {
    nw = w;
    nh = h;
}

void StaticElement::Render() {
    renderer->Render(static_cast<Texture&>(*this));
}
