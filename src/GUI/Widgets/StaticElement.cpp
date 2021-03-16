#include "GUI/Widgets/StaticElement.h"
#include "Rendering/Renderer.h"

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(Rect<int> tp, Texture* t, SDL_Rect& inactive,
                             SDL_Rect& active, size_t id)
    : EventConsumer<Mouse>(),
      tp{tp},
      tex{t},
      inactive{inactive},
      active{active},
      id{id},
      visible{true} {
}

StaticElement::StaticElement(const StaticElement& e)
    : EventConsumer<Mouse>(),
      tp{e.tp},
      tex{e.tex},
      inactive{e.inactive},
      active{e.active},
      id{e.id},
      visible{e.visible} {
}

void StaticElement::Render() {
  if (!visible || !tex)
    return;

  int mouseX = GetMouseX();
  int mouseY = GetMouseY();

  Rendering::Render(*tex, tp.x, tp.y, tp.w, tp.h,
                    DetectMouseOver(mouseX, mouseY) ? &active : &inactive);
}

void StaticElement::onEventReceive(Mouse& event) {
  if (visible && DetectMouseOver(event.GetPosX(), event.GetPosY()) && onEvent) {
    onEvent(id, event);
  }
}

bool StaticElement::DetectMouseOver(int mx, int my) {
  auto target = GetTarget();

  int rx = target.x + tp.x;
  int ry = target.y + tp.y;
  int rw = tp.w;  // TODO
  int rh = tp.h;  // TODO

  return (mx > rx && my > ry && mx < rx + rw && my < ry + rh);
}
