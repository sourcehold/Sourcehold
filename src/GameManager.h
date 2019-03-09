#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include <AnimationHandler.h>

#include <Events/EventHandler.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Rendering/Display.h>
#include <Rendering/Rendering.h>
#include <Rendering/Renderer.h>

#include <Audio/SoundManager.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Events;

        using System::GameOptions;
        using Audio::SoundManager;
        using Rendering::Display;

        /*
         * The game manager class, constructs/destructs
         * and hands over access to core classes
         */
        class GameManager :
            public AnimationHandler,
            public SoundManager,
            public Display
        {
                std::shared_ptr<EventHandler> eventHandler;
                bool running = false;
                double time = 0.0;
                GameOptions &opt;
            public:
                GameManager(GameOptions &opt);
                GameManager(const GameManager &manager);
                ~GameManager();

                bool Running();
                void Update();

                inline double GetTime() { return time; }
                inline std::shared_ptr<EventHandler> GetHandler() { return eventHandler; }
            protected:
        };
    }
}
