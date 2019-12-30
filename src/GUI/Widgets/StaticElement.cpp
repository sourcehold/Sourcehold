#include "GUI/Widgets/StaticElement.h"
#include "Rendering/Renderer.h"

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(double x, double y, Texture* t) :
    EventConsumer<Mouse>()
{
    np.x = x;
    np.y = y;
    tex = t;
}

StaticElement::StaticElement(const StaticElement& elem) :
    EventConsumer<Mouse>()
{
    this->tex = elem.tex;
    this->shown = elem.shown;
    this->np = elem.np;
    this->tp = elem.tp;
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

void StaticElement::SetTexture(Texture* t)
{
    tex = t;
}

void StaticElement::Translate(int x, int y)
{
    np.x = NormalizeX(x);
    np.y = NormalizeY(y);
    tp.x = NormalizeTargetX(x);
    tp.y = NormalizeTargetY(y);
}

void StaticElement::Translate(double x, double y)
{
    tp.x = x;
    tp.y = y;
    np.x = GetTargetWidth() * x;
    np.y = GetTargetHeight() * y;
}

void StaticElement::Scale(int w, int h)
{
    np.w = NormalizeX(w);
    np.h = NormalizeY(h);
    tp.w = NormalizeTargetX(w);
    tp.h = NormalizeTargetY(h);
}

void StaticElement::Scale(double w, double h)
{
    tp.w = w;
    tp.h = h;
    np.w = GetTargetWidth() * w;
    np.h = GetTargetHeight() * h;
}

void StaticElement::Render(std::function<SDL_Rect()> render_fn)
{
    if (!shown || !tex) return;

    DetectMouseOver();

    SDL_Rect elem = render_fn();
    Rendering::Render(*tex, np.x, np.y, /*np.w, np.h,*/ &elem);
}

bool StaticElement::IsClicked()
{
    if (clicked && shown) {
        clicked = false;
        return true;
    }
    return false;
}

static int mouseX = 0, mouseY = 0;
void StaticElement::onEventReceive(Mouse& event)
{
    EventType type = event.GetType();
    if (type == MOTION) {
        mouseX = event.GetPosX();
        mouseY = event.GetPosY();
    }
    else if (type == BUTTONDOWN) {
        if (mouseOver && shown) clicked = true;
    }
}

void StaticElement::DetectMouseOver()
{
    mouseOver = false;

    if (shown) {
        int rw = ToCoordX(tp.w) * GetTargetWidth();
        int rh = ToCoordY(tp.h) * GetTargetHeight();
        int rx = ToCoordX(GetTargetX()) + tp.x * (double)ToCoordX(GetTargetWidth());
        int ry = ToCoordY(GetTargetY()) + tp.y * (double)ToCoordY(GetTargetHeight());

        if (mouseX > rx&& mouseY > ry&& mouseX < rx + rw && mouseY < ry + rh) {
            mouseOver = true;
        }
    }
}
