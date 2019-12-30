#pragma once

#include <vector>
#include <memory>

#include "Rendering/Shapes.h"
#include "GUI/Widgets/Widget.h"

namespace Sourcehold {
    namespace GUI {
        enum class WidgetLayout {
            VERTICAL,
            HORIZONTAL
        };

        using namespace Rendering;
        class Container : public Widget
        {
            std::vector<std::shared_ptr<Widget>> widgets;
            WidgetLayout layout;
            Rect<int> constraints;
        public:
            Container(WidgetLayout l);
            ~Container() {}

            void Update(Rect<int> constraints) override;

            /** Add a widget to the container. */
            void Add(std::shared_ptr<Widget> widget);
        };
    }
}
