#pragma once

#include <cinttypes>
#include <memory>

#include <GameManager.h>

#include <Events/Event.h>
#include <Events/Mouse.h>

#include <Rendering/Texture.h>
#include <Rendering/Font.h>

#include <Audio/Song.h>
#include <Parsers/TgxFile.h>

namespace Sourcehold {
    namespace GUI {
        using namespace Game;
        using namespace Audio;
        using namespace Events;

        /**
         * This handles the credits sequences,
         * the one in the main menu and the endgame credits.
         */
        class Credits : protected EventConsumer<Mouse> {
        public:
            Credits();
            Credits(const Credits&) = delete;
            ~Credits();

            bool Play(bool endgame, bool fadein, bool loop);
        protected:
            void onEventReceive(Mouse &event) override;

            bool playing;
        };
    }
}
