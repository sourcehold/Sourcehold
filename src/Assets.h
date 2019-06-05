#pragma once

#include <cinttypes>
#include <vector>

/**
* Definitions and look-up tables
* related to the game's assets
*/
namespace Sourcehold
{
    namespace Assets
    {
        enum AssetType : uint8_t {
			UNKNOWN,
            BIK, /* Bink video (.bik) file */
            WAV, /* Wav audio (.wav) file */
            RAW, /* Raw PCM audio (.raw) */
            TGX, /* Texture (.tgx) */
            GM1, /* Texture/Animation container (.gm1) */
            ANI, /* Animated cursor (.ani) */
            MAP, /* Map file (.map) */
            MLB, /* String container (.mlb) */
            TXT, /* Audio volume file (.txt) */
            ACT, /* ? */
            BMP,
            MISC,
        };

        enum LocalizedMissionDescription : uint16_t {
        };

        enum LocalizedTextString : uint16_t {
        };

		enum SoundFiles : uint16_t {
		};

        /*
        * The list of known player names
        * to read in the main menu
        */
        #define NAME_INDEX_FEMALE 0
        #define NAME_INDEX_MALE 44
        #define NAME_INDEX_NONSENSE 151
        extern const std::vector<const wchar_t*>lut_names;
	}
}

