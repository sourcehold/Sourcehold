/* A list of all the game's assets */

#pragma once

#include <Config.h>

namespace Sourcehold
{
    namespace Game
    {
        /* Asset types */
        enum Assets {
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
