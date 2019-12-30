#pragma once

#include <string>
#include <cinttypes>
#include <memory>

#include "Events/Event.h"
#include "Events/Keyboard.h"

#include "Parsers/Gm1File.h"

#include "Rendering/Renderer.h"
#include "Rendering/TextureAtlas.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;

        class LineEdit : protected EventConsumer<Keyboard> {
            const uint16_t MAX_LENGTH = 100;
            std::wstring line;
            uint16_t cp = 0;
        public:
            LineEdit() = default;
            LineEdit(const std::wstring& line);
            ~LineEdit();

            void Init();
            void BeginInput();
            void EndInput();
            void Render(int x, int y, int nx);

            inline void SetLine(const std::wstring &line) { this->line = line; cp = line.size(); }
            inline void SetPos(uint16_t cp) { this->cp = cp; }
            inline std::wstring &GetLine() { return line; }
        protected:
            void onEventReceive(Keyboard &event) override;
        };
    }
}
