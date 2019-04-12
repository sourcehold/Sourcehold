#include <StaticElement.h>

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(std::shared_ptr<GameManager> mgr, double x, double y, SDL_Rect r) :
    manager(mgr),
    EventConsumer<Mouse>(mgr->GetHandler())
{
    nx = x;
    ny = y;
    rect = r;
}

StaticElement::StaticElement(const StaticElement &elem) :
    manager(elem.manager),
    EventConsumer<Mouse>(elem.manager->GetHandler())
{
    this->rect = elem.rect;
    this->shown = elem.shown;
    this->nx = elem.nx;
    this->ny = elem.ny;
    this->nw = elem.nw;
    this->nh = elem.nh;
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
    nx = manager->NormalizeX(x);
    ny = manager->NormalizeY(y);
}

void StaticElement::Translate(double x, double y) {
    nx = x;
    ny = y;
}

void StaticElement::Scale(uint32_t w, uint32_t h) {
    nw = manager->NormalizeX(w);
    nh = manager->NormalizeY(h);
}

void StaticElement::Scale(double w, double h) {
    nw = w;
    nh = h;
}

void StaticElement::Render(std::function<Texture&()> render_fn) {
    Texture &elem = render_fn();
    SDL_Rect r = elem.GetRect();
    manager->Render(elem, nx, ny, nw, nh, &r);
}

void StaticElement::Render(Texture &elem, bool whole) {
    SDL_Rect r = elem.GetRect();
    manager->Render(elem, nx, ny, nw, nh, whole ? nullptr : &r);
}

bool StaticElement::IsMouseOver() {
    manager->PushTarget();

    /* TODO: Convert to screen coordinates */
    manager->SetTarget();
    int rx = manager->ToCoordX(nx);
    int ry = manager->ToCoordY(ny);
    int rw = manager->ToCoordX(nw);
    int rh = manager->ToCoordY(nh);

    manager->PopTarget();

    if(mouseX > rx && mouseY > ry && mouseX < rx+rw && mouseY < ry+rh) return true;
    return false;
}

void StaticElement::onEventReceive(Mouse &event) {
    EventType type = event.GetType();

    int tw = manager->GetTargetWidth();
    int th = manager->GetTargetHeight();

    int w = manager->GetWidth();
    int h = manager->GetHeight();

    if(type == MOUSE_MOTION) {
        mouseX = event.GetPosX();
        mouseY = event.GetPosY();
    }
}

