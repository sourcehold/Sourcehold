#include <GUI/StaticElement.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(double x, double y, Texture *t) :
    EventConsumer<Mouse>(GetHandler())
{
    nx = x;
    ny = y;
    tex = t;
    ID = RegisterFrameTick([&](double) { mouseOver = IsMouseOverInternal(); });
}

StaticElement::StaticElement(const StaticElement &elem) :
    EventConsumer<Mouse>(GetHandler())
{
    this->tex = elem.tex;
    this->shown = elem.shown;
    this->nx = elem.nx;
    this->ny = elem.ny;
    this->nw = elem.nw;
    this->nh = elem.nh;
    ID = RegisterFrameTick([&](double) { mouseOver = IsMouseOverInternal(); });
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

void StaticElement::TransparencyCheck(bool check)
{
    transCheck = check;
}

void StaticElement::Render(std::function<SDL_Rect()> render_fn)
{
    if(!shown) return;

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

    int rw = ToCoordX(tw) * GetTargetWidth();
    int rh = ToCoordY(th) * GetTargetHeight();
    int rx = ToCoordX(GetTargetX()) + tx * (double)ToCoordX(GetTargetWidth());
    int ry = ToCoordY(GetTargetY()) + ty * (double)ToCoordY(GetTargetHeight());

    if(mouseX > rx && mouseY > ry && mouseX < rx+rw && mouseY < ry+rh) {
        if(transCheck) {
            /* TODO */
            return true;

            /* Get cursor position on image */
            int ix = mouseX - rx;
            int iy = mouseY - ry;

            /* Check pixel at position */
            if(ix < tex->GetWidth() && iy < tex->GetHeight()) {
                //Uint32 pixel = tex->GetPixel(ix, iy);
                //return !(pixel & (uint32_t)0xFF);
            }
            return false;
        }else {
            return true;
        }
    }

    return false;
}
