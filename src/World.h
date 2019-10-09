#pragma once

#include <memory>

#include "GameMap.h"
#include "GameManager.h"

#include "GUI/UIState.h"
#include "GUI/Ingame.h"

#include "Rendering/Texture.h"

#include "Events/Event.h"
#include "Events/Keyboard.h"
#include "Events/Mouse.h"

namespace Sourcehold {
    namespace Parsers {
        class TgxFile;
        class Gm1File;
    }
    namespace Game {
        struct Scroll {
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

        using namespace GUI;
        using namespace Parsers;
        using namespace Events;

        /**
         * Handles everything related to the game world, including
         * loading, rendering and moving the camera
         */
        class Building;
        class Entity;
        class World : 
            public GameMap,
            private EventConsumer<Keyboard>,
            private EventConsumer<Mouse>,
            private IngameGUI
        {
            ScrollInformation scroll;
        public:
            World();
            World(const World&) = delete;
            ~World();

            UIState Play();
        protected:
            void UpdateCamera();
        private:
            void onEventReceive(Keyboard& keyEvent) override;
            void onEventReceive(Mouse& mouseEvent) override;
        };
    }
}
