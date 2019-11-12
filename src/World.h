#pragma once

#include <memory>

#include "GameMap.h"
#include "GameManager.h"

#include "GUI/UIState.h"
#include "GUI/Ingame.h"

#include "Parsers/Gm1File.h"

#include "Rendering/Texture.h"
#include "Rendering/Camera.h"

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

        // Just testing. Nothing to see here, move along! //
        class Unit {
        public:
            Unit(int x, int y, const char *f) : x(x), y(y) {
                file = GetGm1(std::string("gm/") + f + std::string(".gm1"));
            }

            virtual void Update(double dt) {};

            void Render() {
                Camera& cam = Camera::instance();

                int px = x * 30 - cam.positionX;
                int py = y * 15 - cam.positionY;

                SDL_Rect r = file->GetTextureAtlas()->Get(index);
                Rendering::Render(*file->GetTextureAtlas(), px, py, &r);
            }

            int x, y;
        protected:
            int index=0;
            std::shared_ptr<Gm1File> file;
        };

        class Lord : Unit {
        public:
            Lord(int x, int y) : Unit(x, y, "body_lord")
            {}
        };

        class Oak : Unit {
        public:
            Oak(int x, int y) : Unit(x, y, "tree_oak")
            {}

            void Update(double dt) {
                index = (24 - abs(int(GetTime() * 15.0f) % (2 * 24) - 24));
            }
        };

        /**
         * Handles everything related to the game world, including
         * loading, rendering and moving the camera
         */
        class Building;
        class Entity;
        class World :
            public GameMap,
            private EventConsumer<Keyboard>,
            private EventConsumer<Mouse>
        {
            ScrollInformation scroll;
            IngameGUI gui;
            std::vector<Unit*> units;
        public:
            World();
            World(const World&) = delete;
            ~World();

            UIState Play();
        protected:
            void UpdateCamera(double dt);

            template<class T>
            void Spawn(int x, int y) {
                units.push_back((Unit*)new T( x, y ));
            }
        private:
            void onEventReceive(Keyboard& keyEvent) override;
            void onEventReceive(Mouse& mouseEvent) override;
        };
    }
}
