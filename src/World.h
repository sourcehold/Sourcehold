#pragma once

#include <memory>

#include <GameMap.h>

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
        /**
         * Handles everything related to the game world, including
         * loading, rendering and moving the camera
         */
        class Building;
        class GameManager;
        class World : public GameMap, protected EventConsumer<Keyboard>, protected EventConsumer<Mouse>
        {
            std::shared_ptr<GameManager> manager;
            Texture menubar;
            std::shared_ptr<TgxFile> tgx_right, tgx_bar_bg;
            std::shared_ptr<Gm1File> gm1_scribe, gm1_face, gm1_icons;
            bool ml = false, mr = false, mu = false, md = false;
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
