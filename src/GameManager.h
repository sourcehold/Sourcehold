#pragma once

#include <Config.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Rendering/Display.h>
#include <Rendering/Rendering.h>
#include <Rendering/Renderer.h>

#include <Sound/Sound.h>

namespace Sourcehold
{
    namespace Game
    {
        class GameManager : public Rendering::Display, public Sound::Sound
        {
            public:
                GameManager();
                GameManager(const GameManager &manager);
                ~GameManager();
            protected:
                void Init();
        };
    }
}
