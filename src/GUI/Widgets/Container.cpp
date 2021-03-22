#include "GUI/Widgets/Container.h"

using namespace Sourcehold::GUI;

Container::Container(WidgetLayout l) : layout{l} {
}

void Container::Update(Rect<int> constraints) {
  /**
   * Update the constraints of all member widgets
   * based on the layout of the container.
   */
  this->constraints = constraints;
  for (int i = 0; i < widgets.size(); i++) {
    const auto& widget = widgets.at(i);

    // Partition the available space evenly //
    Rect<int> dim;
    if (layout == WidgetLayout::HORIZONTAL) {
      int partX = constraints.w / widgets.size();
      dim.x = constraints.x + partX * i;
      dim.y = constraints.y;
      dim.w = partX;
      dim.h = constraints.h;
    }
    else {
      int partY = constraints.h / widgets.size();
      dim.x = constraints.x;
      dim.y = constraints.y + partY * i;
      dim.w = constraints.w;
      dim.h = partY;
    }

    widget->Update(dim);
  }
}
