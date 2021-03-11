#include "GUI/Widgets/StaticElement.h"
#include "Rendering/Renderer.h"

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(Rect<int> tp, Texture* t, SDL_Rect& inactive, SDL_Rect& active, size_t id) :
    EventConsumer<Mouse>(),
    tex_{t},
    tp_{tp},
    id_{id},
    inactive_{inactive},
    active_{active},
    visible_{true}
{}

StaticElement::StaticElement(const StaticElement& e) :
    EventConsumer<Mouse>(),
    tex_{e.tex_},
    tp_{e.tp_},
    id_{e.id_},
    inactive_{e.inactive_},
    active_{e.active_},
    visible_{e.visible_}
{}

void StaticElement::Render()
{
    if (!visible_ || !tex_) return;

    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    Rendering::Render(*tex_, tp_.x, tp_.y, tp_.w, tp_.h, DetectMouseOver(mouseX, mouseY) ? &active_ : &inactive_);
}

void StaticElement::onEventReceive(Mouse& event)
{
    if (visible_ && DetectMouseOver(static_cast<int>(event.GetPosX()), static_cast<int>(event.GetPosY())) && onEvent_) {
        onEvent_(id_, event);
    }
}

bool StaticElement::DetectMouseOver(int mx, int my)
{
    auto target = GetTarget();

    int rx = target.x + tp_.x;
    int ry = target.y + tp_.y;
    int rw = tp_.w; // TODO
    int rh = tp_.h; // TODO

    return (mx > rx && my > ry && mx < rx + rw && my < ry + rh);
}
