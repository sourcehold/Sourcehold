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
        void RenderText(const std::wstring& text, uint32_t x, uint32_t y, Font type = FONT_SMALL, bool illumination = false, double scaleFactor = 1.0);
        /* Render text inside the rect */
        void RenderText(const std::wstring& text, Rect<uint32_t> bounds, Align al = Align::CENTER, Font type = FONT_SMALL, bool illumination = false);
        std::pair<uint32_t, uint32_t> GetStringPixelDim(const std::wstring& text, Font type);
    }
}
