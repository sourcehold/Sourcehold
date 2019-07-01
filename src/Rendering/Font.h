#pragma once

#include <cinttypes>
#include <memory>
#include <string>

namespace Sourcehold {
    namespace Game {
        class GameManager;
    }
    namespace Rendering {
        enum Font : uint8_t {
            FONT_SLANTED = 0,
            FONT_SMALL = 1,
            FONT_LARGE = 2
        };

        bool LoadFonts();
        void UnloadFonts();
        void RenderText(const std::wstring& text, int32_t x, int32_t y, Font type = FONT_SMALL, bool illumination = false);
        void RenderText(const std::wstring& text, double x, double y, Font type = FONT_SMALL, bool illumination = false);
    }
}
