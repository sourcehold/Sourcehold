#pragma once

#include <cstdlib>

#include <Config.h>
#include <Display.h>
#include <MainMenu.h>
#include <MlbFile.h>
#include <Gm1File.h>
#include <TgxFile.h>
#include <Sound.h>
#include <Rendering.h>
#include <BinkVideo.h>

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
