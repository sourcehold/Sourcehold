#pragma once

#include <cinttypes>
#include <memory>
#include <vector>
#include <string>

#include <Rendering/Font.h>
#include <Rendering/Shapes.h>

#include <Parsers/HlpFile.h>
#include <Parsers/TgxFile.h>

namespace Sourcehold {
    namespace GUI {
        using namespace Parsers;
        using namespace Rendering;

        enum class PicPosition : uint8_t {
            LEFT,
            RIGHT,
            CENTRE
        };

        struct LayoutElement {
            enum Type {
                LINE,
                PIC,
                PARAGRAPH,
                LINEBREAK
            } type;
            PicPosition pos;
            Font font;
            std::wstring text;
            std::shared_ptr<TgxFile> pic;
            int x, y;
        };

        /**
         * Handles GUI layouts containing text and
         * images, which can be created manually
         * or by using the stronghold_help.hlp file.
         */
        class Layout {
        public:
            Layout();
            Layout(HlpSection *hlp, Rect<double> bounds = { 0.0, 0.0, 1.0, 1.0 });
            ~Layout();

            void CreateFromHlp(HlpSection *hlp);
            void Destroy();
            void Render(double x, double y);
            void Render(int x, int y);

            /* Modification calls */
            void SetFont(Font index);
            void AddText(const std::wstring &str);
            void AddPic(std::shared_ptr<TgxFile> pic, PicPosition pos);
            void NewParagraph();

            inline void SetBounds(Rect<double> bounds) { this->bounds = bounds;  }
        protected:
            Rect<double> bounds;
            std::vector<LayoutElement> elems;
            Font currentFont = FONT_SMALL;
            uint32_t width = 0, height = 0;
            std::vector<std::shared_ptr<TgxFile>> pics;
        };
    }
}
