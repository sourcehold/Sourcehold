#pragma once

#include <cstdlib>
#include <string>

#include <Config.h>
#include <Assets.h>
#include <GameManager.h>
#include <MainMenu.h>

#include <Audio/SoundManager.h>
#include <Audio/Playlist.h>

#include <Rendering/Tileset.h>

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
                Parsers::TgxFile tgx;
                Rendering::Texture tex1, tex2, tex3, tex4;
                Audio::AudioSource vidSound;
                Rendering::Texture vidFrame;
            public:
                Game();
                ~Game();

                bool Init(System::CmdLineOptions &opt);
                int Start();
            protected:
        };
    }
}
