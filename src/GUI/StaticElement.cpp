#include "GUI/StaticElement.h"
#include "Rendering/Renderer.h"

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(double x, double y, Texture *t) :
    EventConsumer<Mouse>()
{
    nx = x;
    ny = y;
    tex = t;
}

StaticElement::StaticElement(const StaticElement &elem) :
    EventConsumer<Mouse>()
{
    this->tex = elem.tex;
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

void StaticElement::SetTexture(Texture *t)
{
    tex = t;
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
}

void StaticElement::Scale(double w, double h)
{
    tw = w;
    th = h;
    nw = GetTargetWidth() * w;
    nh = GetTargetHeight() * h;
}

void StaticElement::Render(std::function<SDL_Rect()> render_fn)
{
    if(!shown || !tex) return;

    DetectMouseOver();

    SDL_Rect elem = render_fn();
    Rendering::Render(*tex, nx, ny, /*nw, nh,*/&elem);
}

bool StaticElement::IsClicked()
{
    if(clicked && shown) {
        clicked = false;
        return true;
    }
    return false;
}

static int mouseX=0, mouseY=0;
void StaticElement::onEventReceive(Mouse &event)
{
    EventType type = event.GetType();
    if(type == MOTION) {
        mouseX = event.GetPosX();
        mouseY = event.GetPosY();
    }
    else if(type == BUTTONDOWN) {
        if(mouseOver && shown) clicked = true;
    }
}

void StaticElement::DetectMouseOver()
{
    mouseOver = false;

    if(shown) {
        int rw = ToCoordX(tw) * GetTargetWidth();
        int rh = ToCoordY(th) * GetTargetHeight();
        int rx = ToCoordX(GetTargetX()) + tx * (double)ToCoordX(GetTargetWidth());
        int ry = ToCoordY(GetTargetY()) + ty * (double)ToCoordY(GetTargetHeight());

        if(mouseX > rx && mouseY > ry && mouseX < rx+rw && mouseY < ry+rh) {
            mouseOver = true;
        }
    }
}
