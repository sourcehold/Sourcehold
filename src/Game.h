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

namespace OpenSH
{
    namespace Game
    {
        class Game : public Display
        {
                std::string data_folder;
                Parsers::TgxFile tgx;
            public:
                Game();
                ~Game();

                bool Init(int argc, char **argv);
                int MainLoop();
            protected:
        };
    }
}
