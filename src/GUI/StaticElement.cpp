#include <GUI/StaticElement.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(double x, double y, SDL_Rect r) :
    EventConsumer<Mouse>(GetHandler())
{
    nx = x;
    ny = y;
    rect = r;
}

StaticElement::StaticElement(const StaticElement &elem) :
    EventConsumer<Mouse>(GetHandler())
{
    this->rect = elem.rect;
    this->shown = elem.shown;
    this->nx = elem.nx;
    this->ny = elem.ny;
    this->nw = elem.nw;
    this->nh = elem.nh;
}

StaticElement::~StaticElement()
{

}

void StaticElement::Hide()
{
    shown = false;
}

void StaticElement::Show()
{
    shown = true;
}

void StaticElement::Destroy()
{

}

void StaticElement::Translate(int x, int y)
{
    nx = NormalizeX(x);
    ny = NormalizeY(y);
    tx = NormalizeTargetX(x);
    ty = NormalizeTargetY(y);
}

void StaticElement::Translate(double x, double y)
{
    tx = x;
    ty = y;
    nx = GetTargetWidth() * x;
    ny = GetTargetHeight() * y;
}

void StaticElement::Scale(int w, int h)
{
    nw = NormalizeX(w);
    nh = NormalizeY(h);
    tw = NormalizeTargetX(w);
    th = NormalizeTargetY(h);

    scaled = true;
}

void StaticElement::Scale(double w, double h)
{
    tw = w;
    th = h;
    nw = GetTargetWidth() * w;
    nh = GetTargetHeight() * h;

    scaled = true;
}

void StaticElement::Render(std::function<Texture&()> render_fn)
{
    if(!shown) return;

    Texture &elem = render_fn();
    SDL_Rect r = elem.GetRect();
    Rendering::Render(elem, nx, ny, nw, nh, &r);
}

void StaticElement::Render(Texture &elem, bool whole)
{
    if(!shown) return;

    SDL_Rect r = elem.GetRect();
    Rendering::Render(elem, nx, ny, nw, nh, whole ? nullptr : &r);
}

bool StaticElement::IsMouseOver()
{
    if(!shown) return false;

    int rw = ToCoordX(tw) * GetTargetWidth();
    int rh = ToCoordY(th) * GetTargetHeight();
    int rx = ToCoordX(GetTargetX()) + tx * (double)ToCoordX(GetTargetWidth());
    int ry = ToCoordY(GetTargetY()) + ty * (double)ToCoordY(GetTargetHeight());

    if(mouseX > rx && mouseY > ry && mouseX < rx+rw && mouseY < ry+rh) return true;
    return false;
}

bool StaticElement::IsClicked()
{
    if(clicked && shown) {
        clicked = false;
        return true;
    }
    return false;
}

void StaticElement::onEventReceive(Mouse &event)
{
    EventType type = event.GetType();
    if(type == MOUSE_MOTION) {
        mouseX = event.GetPosX();
        mouseY = event.GetPosY();
    }
    else if(type == MOUSE_BUTTONDOWN) {
        if(IsMouseOver() && shown) clicked = true;
    }
}
