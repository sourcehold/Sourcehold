/* A list of all the game's assets */

#pragma once

#include <Config.h>

namespace Sourcehold
{
    namespace Assets
    {
        /* Asset types */
        enum Asset {
            BINK_VIDEO, /* Bink video (.bik) file */
            WAVEFILE, /* Wav audio (.wav) file */
            RAWFILE, /* Raw PCM audio (.raw) */
            TGXFILE, /* Texture (.tgx) */
            GM1FILE, /* Texture/Animation container (.gm1) */
            MAPFILE, /* Map file (.map) */
            MLBFILE, /* String container (.mlb) */
        };

        namespace Units
        {
            
        }
    }
}
