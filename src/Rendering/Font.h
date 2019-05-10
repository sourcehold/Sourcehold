#pragma once

#include <cinttypes>
#include <memory>
#include <string>

namespace Sourcehold
{
    namespace Game
    {
        class GameManager;
    }
    namespace Rendering
    {
        enum Font : uint8_t {
            FONT_SLANTED = 0,
            FONT_SMALL = 1,
            FONT_LARGE = 2
        };

        bool LoadFonts(std::shared_ptr<Game::GameManager> mgr);
        void UnloadFonts();
        void RenderText(const std::wstring& text, int32_t x, int32_t y, double scaleFactor = 1, Font type = FONT_SMALL, bool illumination = false);
        void RenderText(const std::wstring& text, double x, double y, double scaleFactor = 1, Font type = FONT_SMALL, bool illumination = false);
    }
}
