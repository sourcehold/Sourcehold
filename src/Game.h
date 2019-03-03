#pragma once

#include <cstdlib>
#include <string>
#include <memory>

#include <cxxopts.hpp>

#include <Config.h>
#include <MainMenu.h>

#include <Assets.h>
#include <GameManager.h>
#include <Building.h>
#include <AnimationHandler.h>

#include <Audio/SoundManager.h>
#include <Audio/Playlist.h>

#include <Rendering/Tileset.h>
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
        using System::GameOptions;

        class Game :
            public GameManager,
            public std::enable_shared_from_this<GameManager>
        {
            public:
                Game(GameOptions &opt);
                ~Game();

                int Start();
            protected:
        };
    }
}
