#include "GUI/Widgets/Container.h"

using namespace Sourcehold::GUI;

Container::Container(WidgetLayout l) : layout_{l} {}

void Container::Update(Rect<int> constraints)
{
    /**
     * Update the constraints of all member widgets
     * based on the layout of the container.
     */
    constraints_ = constraints;
    for (int i = 0; i < static_cast<int>(widgets_.size()); i++) {
        const auto& widget = widgets_.at(static_cast<size_t>(i));

        // Partition the available space evenly //
        Rect<int> dim;
        if (layout_ == WidgetLayout::HORIZONTAL) {
            int partX = constraints_.w / static_cast<int>(widgets_.size());
            dim.x = constraints_.x + partX*i;
            dim.y = constraints_.y;
            dim.w = partX;
            dim.h = constraints_.h;
        }
        else {
            int partY = constraints_.h / static_cast<int>(widgets_.size());
            dim.x = constraints_.x;
            dim.y = constraints_.y + partY*i;
            dim.w = constraints_.w;
            dim.h = partY;
        }

        widget->Update(dim);
    }
}

