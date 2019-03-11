#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <Config.h>

namespace Sourcehold
{
    namespace Assets
    {
        enum AssetType : uint8_t {
            BINK_VIDEO, /* Bink video (.bik) file */
            WAVEFILE, /* Wav audio (.wav) file */
            RAWFILE, /* Raw PCM audio (.raw) */
            TGXFILE, /* Texture (.tgx) */
            GM1FILE, /* Texture/Animation container (.gm1) */
            MAPFILE, /* Map file (.map) */
            MLBFILE, /* String container (.mlb) */
        };
    }
}

