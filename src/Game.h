#pragma once

#include <cstdlib>

#include <Config.h>
#include <MainMenu.h>

#include <Rendering/Display.h>
#include <Rendering/Rendering.h>

#include <Parsers/MlbFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>
#include <Parsers/BinkVideo.h>

#include <Sound/Sound.h>

namespace OpenSH
{
    namespace Game
    {
        class Game : public Display, public Sound::Sound
        {
                std::string data_folder;
                Parsers::TgxFile tgx;
                Rendering::BinkVideo bik;
            public:
                Game();
                ~Game();

                bool Init(int argc, char **argv);
                int MainLoop();
            protected:
        };
    }
}
