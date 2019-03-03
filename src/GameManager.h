#pragma once

#include <memory>

#include <Config.h>
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
        using System::GameOptions;
        using Audio::SoundManager;
        using Rendering::Display;
        using Events::EventHandler;

        class GameManager :
            public AnimationHandler,
            public SoundManager,
            public Display
        {
                bool running = false;
                GameOptions &opt;
                std::shared_ptr<EventHandler> eventHandler;
            public:
                GameManager(GameOptions &opt);
                GameManager(const GameManager &manager);
                ~GameManager();

                bool Running();
                void Update();

                inline std::shared_ptr<EventHandler> GetHandler() { return eventHandler; }
            protected:
        };
    }
}
