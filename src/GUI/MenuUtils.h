#pragma once

#include <string>

namespace Sourcehold {
    namespace GUI {
        bool InitializeUtils();
        void RenderMenuText(const std::wstring &text);
        void RenderMenuBorder();
    }
}
