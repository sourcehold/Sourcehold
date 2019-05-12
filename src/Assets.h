#pragma once

#include <cinttypes>
#include <string>
#include <memory>
#include <unordered_map>

#include <Config.h>

#include <System/FileUtil.h>
#include <System/Logger.h>

#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace Parsers {
        class TgxFile;
        class Gm1File;
        class AniFile;
        class MlbFile;
    }

    namespace Audio {
        class AudioSource;
    }

    namespace Rendering {
        class BinkVideo;
    }

    namespace Assets
    {
        using namespace Parsers;
        using namespace Rendering;
        using namespace Audio;

        enum AssetType : uint8_t {
            BINK_VIDEO, /* Bink video (.bik) file */
            WAVEFILE, /* Wav audio (.wav) file */
            RAWFILE, /* Raw PCM audio (.raw) */
            TGXFILE, /* Texture (.tgx) */
            GM1FILE, /* Texture/Animation container (.gm1) */
            ANIFILE, /* Animated cursor (.ani) */
            MAPFILE, /* Map file (.map) */
            MLBFILE, /* String container (.mlb) */
            TXTFILE, /* Audio volume file (.txt) */
            ACTFILE,
            BMPFILE,
            UNKNOWN,
        };

        enum LocalizedMissionDescription : uint16_t {
        };

        enum LocalizedTextString : uint16_t {
        };
    }
}

