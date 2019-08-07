#pragma once

#include <memory>
#include <cinttypes>
#include <vector>
#include <string>

#include <Events/Event.h>
#include <Events/Mouse.h>

#include <Parsers/TgxFile.h>
#include <Assets.h>

namespace Sourcehold {
    using Parsers::TgxFile;

    using namespace Events;
    using namespace Assets;

    namespace GUI {
        class NarrScreen : protected EventConsumer<Mouse> {
            std::vector<std::wstring> lines;
            std::shared_ptr<TgxFile> tgx_bg, tgx_anim[26], tgx_anim2[13];
        public:
            NarrScreen(TextSection sec);
            ~NarrScreen();

            bool Begin();
        protected:
            void onEventReceive(Mouse &mouse) override;
            void RenderFlameAnim(int px, int py, int index);
        };
    }
}
