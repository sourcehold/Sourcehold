#pragma once

#include <memory>

#include <GameMap.h>
#include <StaticElement.h>

#include <Rendering/Texture.h>

#include <Events/Keyboard.h>
#include <Events/Mouse.h>

namespace Sourcehold
{
    namespace Parsers
    {
        class TgxFile;
        class Gm1File;
    }
    namespace Game
    {
        class Scroll {
        public:
            bool shouldScroll = false;
            bool setByKeyboard = false;
            bool setByMouse = false;

            explicit operator bool() {
                return shouldScroll;
            }
        };

        struct ScrollInformation {
            Scroll up;
            Scroll down;
            Scroll right;
            Scroll left;
        };

        /**
         * Handles everything related to the game world, including
         * loading, rendering and moving the camera
         */
        class Building;
        class GameManager;
        class World : public GameMap, protected EventConsumer<Keyboard>, protected EventConsumer<Mouse>
        {
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_right, tgx_bar_bg;
            std::shared_ptr<Gm1File> gm1_scribe, gm1_face, gm1_icons;
            ScrollInformation scroll;
            StaticElement ui_disk, ui_info, ui_delete, ui_revert;
        public:
            World(std::shared_ptr<GameManager> mgr);
            World(const World&) = delete;
            ~World();

            int Play();
        protected:
            void RenderMenubar();
            void UpdateCamera();
            void onEventReceive(Keyboard &keyEvent) override;
            void onEventReceive(Mouse &mouseEvent) override;
        };
    }
}
