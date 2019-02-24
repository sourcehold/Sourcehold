#pragma once

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
        class GameManager :
            public AnimationHandler,
            public Rendering::Display,
            public Audio::SoundManager
        {
            public:
                GameManager();
                GameManager(const GameManager &manager);
                ~GameManager();

                bool Running();
                void Update();
            protected:
                void Init();
        };
    }
}
