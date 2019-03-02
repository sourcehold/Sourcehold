#pragma once

#include <memory>

#include <Config.h>
#include <AnimationHandler.h>

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

        class GameManager :
            public AnimationHandler,
            public SoundManager
        {
                GameOptions &opt;
            public:
                GameManager(GameOptions &opt);
                GameManager(const GameManager &manager);
                ~GameManager();

                void Update();
            protected:
                bool Running();
                std::shared_ptr<Display> renderer;
        };
    }
}
