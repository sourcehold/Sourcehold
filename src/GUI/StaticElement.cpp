#include <GUI/StaticElement.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(double x, double y, Texture *t) :
    EventConsumer<Mouse>()
{
    nx = x;
    ny = y;
    tex = t;
    ID = RegisterFrameTick([&](double) {
        mouseOver = IsMouseOverInternal();
    });
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
    ID = RegisterFrameTick([&](double) {
        mouseOver = IsMouseOverInternal();
    });
}

StaticElement::~StaticElement()
{
    DeregisterFrameTick(ID);
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

void StaticElement::SetTexture(Texture *t)
{
    /* TODO: check pointer, reference? */
    tex = t;
}

void StaticElement::Translate(int x, int y)
{
    nx = NormalizeX(x);
    ny = NormalizeY(y);
    tx = NormalizeTargetX(x);
    ty = NormalizeTargetY(y);
    tpX = GetTargetX();
    tpY = GetTargetY();
    tdW = GetTargetWidth();
    tdH = GetTargetHeight();
}

void StaticElement::Translate(double x, double y)
{
    tx = x;
    ty = y;
    nx = GetTargetWidth() * x;
    ny = GetTargetHeight() * y;
    tpX = GetTargetX();
    tpY = GetTargetY();
    tdW = GetTargetWidth();
    tdH = GetTargetHeight();
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

void StaticElement::Render(std::function<SDL_Rect()> render_fn)
{
    if(!shown || !tex) return;

    SDL_Rect elem = render_fn();
    Rendering::Render(*tex, nx, ny, nw, nh, &elem);
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
        if(mouseOver && shown) clicked = true;
    }
}

bool StaticElement::IsMouseOverInternal()
{
    if(!shown) return false;

    /* Target x/y/w/h unknown, since called as tick function */
    int rw = ToCoordX(tw) * tdW;
    int rh = ToCoordY(th) * tdH;
    int rx = ToCoordX(tpX) + tx * (double)ToCoordX(tdW);
    int ry = ToCoordY(tpY) + ty * (double)ToCoordY(tdH);

    if(mouseX > rx && mouseY > ry && mouseX < rx+rw && mouseY < ry+rh) {
        return true;
    }

    return false;
}
