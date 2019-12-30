#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <utility>

#include "Rendering/Shapes.h"

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

        enum class Align : uint8_t {
            LEFT,
            CENTER,
            RIGHT
        };

        void LoadFonts();
        void UnloadFonts();
        void RenderText(const std::wstring& text, int32_t x, int32_t y, Font type = FONT_SMALL, bool illumination = false, double scaleFactor = 1.0);
        /* Render text inside the rect */
        void RenderText(const std::wstring& text, Rect<int> bounds, Align al = Align::CENTER, Font type = FONT_SMALL, bool illumination = false);
        std::pair<uint32_t, uint32_t> GetStringPixelDim(const std::wstring& text, Font type);
    }
}
