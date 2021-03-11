#pragma once

#include <string>
#include <cinttypes>
#include <memory>

#include "Events/Event.h"
#include "Events/Keyboard.h"

#include "Parsers/Gm1File.h"

#include "Rendering/Renderer.h"
#include "Rendering/TextureAtlas.h"

#include "GUI/Widgets/Widget.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;

        class LineEdit : protected EventConsumer<Keyboard>, public Widget {
            const uint16_t MAX_LENGTH = 100;
            const uint16_t LINE_SEGMENT_W = 10;
            std::wstring line_;
            int nx_,cp_; // number of line segments, character position
        public:
            LineEdit(int nx, const std::wstring& line = L"");
            ~LineEdit();

            void Init();
            void BeginInput();
            void EndInput();
            void Update(Rect<int> constraints) override;

            inline void SetLine(const std::wstring &line) {
              line_ = line;
              cp_ = static_cast<int>(line.size());
            }
            inline void SetPos(uint16_t cp) { cp_ = cp; }
            inline std::wstring &GetLine() { return line_; }
        protected:
            void onEventReceive(Keyboard &event) override;
        };
    }
}
