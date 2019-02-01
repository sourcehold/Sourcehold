#pragma once

#include <cstdlib>

#include <Config.h>
#include <Display.h>
#include <MainMenu.h>
#include <MlbFile.h>

namespace OpenSH
{
    namespace Game
    {
        class Game : public Display
        {
                std::string data_folder;
            public:
                Game();
                ~Game();

                bool Init(int argc, char **argv);
                int MainLoop();
            protected:
        };
    }
}
