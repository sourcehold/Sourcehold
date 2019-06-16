#pragma once

#include <string>

namespace Sourcehold {
    namespace GUI {
        bool InitializeUtils();

        /**
         * Renders the given string to fit the main menu
         * rectangle.
         * TODO: replace with complete text box function
         */
        void RenderMenuText(const std::wstring &text);

        /**
         * Render the border around the screen. This asset
         * only exists for the HD edition to prevent scaling
         * the interface to fit the screen.
         */
        void RenderMenuBorder();
    }
}
