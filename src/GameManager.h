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

#include <Audio/Audio.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Events;
        using namespace System;
        using namespace Rendering;

        class GameManager :
            public AnimationHandler,
            public Display
        {
                std::shared_ptr<EventHandler> eventHandler;
                bool running = false;
                double time = 0.0;
                GameOptions &opt;
            public:
                GameManager(GameOptions &opt);
                GameManager(const GameManager&) = delete;
                ~GameManager();

                bool Running();
                void Update();

                inline double GetTime() { return time; }
                inline std::shared_ptr<EventHandler> GetHandler() { return eventHandler; }
            protected:
        };
    }
}
