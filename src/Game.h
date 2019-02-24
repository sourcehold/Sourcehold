#pragma once

#include <cstdlib>
#include <string>

#include <Config.h>

#include <Assets.h>
#include <GameManager.h>
#include <MainMenu.h>
#include <Building.h>
#include <AnimationHandler.h>

#include <Audio/SoundManager.h>
#include <Audio/Playlist.h>

#include <Rendering/Tileset.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/BinkVideo.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Parsers/MlbFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>
#include <Parsers/VolumeTxt.h>

namespace Sourcehold
{
    namespace Game
    {
        class Game : public GameManager
        {
                MainMenu menu;
            public:
                Game();
                ~Game();

                bool Init(System::CmdLineOptions &opt);
                int Start();
            protected:
        };
    }
}
