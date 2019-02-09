#pragma once

#include <cstdlib>

#include <Config.h>
#include <GameManager.h>
#include <MainMenu.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Parsers/MlbFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>
#include <Parsers/BinkVideo.h>
#include <Parsers/VolumeTxt.h>

namespace Sourcehold
{
    namespace Game
    {
        class Game : public GameManager
        {
                MainMenu menu;
                std::string data_folder;
                Rendering::BinkVideo bik;
            public:
                Game();
                ~Game();

                bool Init(System::CmdLineOptions &opt);
                int Start();
            protected:
        };
    }
}
