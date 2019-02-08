#pragma once

#include <cstdlib>

#include <Config.h>
#include <MainMenu.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Rendering/Display.h>
#include <Rendering/Rendering.h>

#include <Parsers/MlbFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>
#include <Parsers/BinkVideo.h>
#include <Parsers/VolumeTxt.h>

#include <Sound/Sound.h>

namespace Sourcehold
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

                bool Init(System::CmdLineOptions &opt);
                int MainLoop();
            protected:
        };
    }
}
