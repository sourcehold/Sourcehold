#pragma once

#include <cinttypes>
#include <vector>

/**
 * Definitions and look-up tables
 * related to the game's assets
 */
namespace Sourcehold {
    namespace Assets {
        // ----------------------------------------------------

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

        // ----------------------------------------------------
        // Map-related

        enum MapDimension : uint8_t {
            WORLD_160,
            WORLD_200,
            WORLD_300,
            WORLD_400
        };

        enum class Building : uint16_t {
            SAXON_HALL,
            WOODEN_KEEP,
            STONE_KEEP,
            FORTRESS,
            STRONGHOLD,
            WOODEN_PLATFORM,
            PERIMETER_TURRET,
            DEFENCE_TURRET,
            SQUARE_TOWER,
            ROUND_TOWER,
            STONE_WALL,
            WOODEN_WALL,
            WOOD_BARRACKS,
            BARRACKS,
            ARMOURY,
            SMALL_WOODEN_GATEHOUSE,
            SMALL_STONE_GATEHOUSE,
            LARGE_STONE_GATEHOUSE,
            DIG_MOAT,
            DRAWBRIDGE,
            BRAZIER,
            KILLING_PITS,
            PITCH_DITCH,
            CAGED_WAR_DOGS,
            STABLES,
            ENGINEERS_GUILD,
            TUNNELERS_GUILD,
            OIL_SMELTER,
            WELL,
            CATAPULT,
            TREBUCHET,
            SIEGE_TOWER,
            BATTERING_RAM,
            PORTABLE_SHIELD,
            MANGONEL,
            BALLISTA,
            STOCKPILE,
            QUARRY,
            IRON_MINE,
            PITCH_RIG,
            MARKETPLACE,
            BLACKSMITHS_WORKSHOP,
            ARMOURERS_WORKSHOP,
            TANNERS_WORKSHOP,
            FLETCHERS_WORKSHOP,
            POLETURNERS_WORKSHOP,
            HUNTERS_POST,
            WHEAT_FARM,
            APPLE_ORCHARD,
            HOPS_FARM,
            DAIRY_FARM,
            GRANARY,
            MILL,
            BAKERY,
            BREWERY,
            INN,
            WOODCUTTER,
            HOVEL,
            APOTHECARY,
            CHAPEL,
            CHURCH,
            CATHEDRAL,
            GOOD_THINGS,
            BAD_THINGS
        };

        enum class Unit : uint8_t {
            ARCHER,
            CROSSBOWMAN,
            SPEARMAN,
            PIKEMAN,
            MACEMAN,
            SWORDSMAN,
            KNIGHT,
            MONK,
            ENGINEER,
            LADDERMAN,
            TUNNELER,
            CATAPULT,
            TREBUCHET,
            SIEGE_TOWER,
            BATTERING_RAM,
            PORTABLE_SHIELD
        };

        // ----------------------------------------------------
        // Text-related

        enum class Message : uint8_t {
            ANIMAL_TRACKS,
            PACKS_OF_WOLVES,
            WOLVES_ON_PROWL,
            HERE_THEY_COME_AGAIN,
            WELL_HAVE_TO_WORK_FAST,
            THE_PIG_HAS_TAKEN,
            SHIPMENT_OF_ALE,
            TROOPS_ARRIVED,
            RETAKEN_THE_COUNTY,
            LONGARM_LEFT,
            CROSSBOW_DESIGN,
            BUILD_CROSSBOWS,
            FOUND_GOLD,
            RETURNED_WITH_THE_KING,
            TAKEN_BACK_CONTROL,
            MARCH_CONTINUES,
            KING_WALKED_UNOPPOSED,
            HIS_MAJESTY_THANKS_YOU,
            YOU_FAILED_TO_DELIVER,
            SHIPMENT_DUE_5,
            SHIPMENT_DUE_4,
            SHIPMENT_DUE_3,
            SHIPMENT_DUE_2,
            SHIPMENT_DUE_1,
            KING_NOTES_WELL,
            CASTLE_IS_LOST,
            DONE_WELL_THUS_FAR,
            ENEMY_TOO_GREAT,
            KINGS_ARCHERS_ARRIVED
        };

        enum LocalizedMissionDescription : uint16_t {
        };

        enum LocalizedTextString : uint16_t {
        };

        // ----------------------------------------------------

        enum SoundFiles : uint16_t {
        };

        // ----------------------------------------------------
        // List of known player names

#define NAME_INDEX_FEMALE 0
#define NAME_INDEX_MALE 44
#define NAME_INDEX_NONSENSE 151
        extern const std::vector<const wchar_t*>lut_names;
    }
}

