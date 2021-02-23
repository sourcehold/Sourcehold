#pragma once

#include <cinttypes>
#include <vector>

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

enum MapDimension : uint8_t { WORLD_160, WORLD_200, WORLD_300, WORLD_400 };

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

enum class MapType : uint8_t { SIEGE = 0x00, INVASION, ECONOMIC, LANDSCAPE };

// ----------------------------------------------------
// Text-related

enum class TextColor : uint8_t {
  LIGHTBLUE,
  DARKORANGE,
  LIGHTORANGE,
  ORANGE,
  GREY,
  CYAN,
  YELLOW,
  GREEN,
  BLUE,
  WHITE,
  BLACK
};

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

enum MissionDescription : uint32_t {};

enum class NPC : uint8_t { WOLF, PIG, SNAKE, RAT };

enum TextSection : uint16_t {
  T_COPYRIGHT = 0,
  T_MONTHS,
  T_GOODS_TYPES,
  T_POPULARITY,
  T_GOODS,
  T_MAIN_MENU,
  T_LANGUAGE,
  T_START_TEXT,
  T_BUILDINGS,
  T_BUBBLE_TEXT,
  T_FEEDBACK,
  T_MAP_EDITOR,
  T_DEMO_SCORE,
  T_UNUSED_1,
  T_SCRIBE_MENU,
  T_UNUSED_2,
  T_UNUSED_3,
  T_UNUSED_4,
  T_INSIDE_BUILDING,
  T_SAXON_HALL,
  T_INN,
  T_UNUSED_5,
  T_BARRACKS,
  T_GRANARY,
  T_HOVEL,
  T_WOODCUTTERS_HUT,
  T_OX_TETHER,
  T_IRON_MINE,
  T_PITCH_RIG,
  T_HUNTERS_POST,
  T_STOCKPILE,
  T_ARMOURY,
  T_FLETCHERS_WORKSHOP,
  T_BLACKSMITHS_WORKSHOP,
  T_POLETURNERS_WORKSHOP,
  T_ARMOURERS_WORKSHOP,
  T_TANNERS_WORKSHOP,
  T_BAKERY,
  T_BREWERY,
  T_QUARRY,
  T_QUARRY_STONE_PILE,
  T_APOTHECARY,
  T_ENGINEERS_GUILD,
  T_TUNNELERS_GUILD,
  T_MARKETPLACE,
  T_WELL,
  T_OIL_SMELTER,
  T_SIEGE_TENT,
  T_WHEAT_FARM,
  T_HOPS_FARM,
  T_APPLE_ORCHARD,
  T_DAIRY_FARM,
  T_MILL,
  T_STABLES,
  T_CHAPEL,
  T_GATEHOUSE,
  T_DRAWBRIDGE,
  T_POSTERN_GATE,
  T_TUNNEL_ENTRANCE,
  T_CAMP_GROUND,
  T_SIGN_POST,
  T_KILLING_PIT,
  T_CATAPULT,
  T_TREBUCHET,
  T_UNUSED_6,
  T_TOWER,
  T_GALLOWS,
  T_STOCKS,
  T_WITCH_HOIST,
  T_MAYPOLE,
  T_UNUSED_7,
  T_TRAINING_GROUND,
  T_GARDEN,
  T_UNUSED_8,
  T_GAME_OPTIONS,
  T_HELP_SYSTEM,
  T_MULTI_PLAYER,
  T_PANEL_FEEDBACK,
  T_STRUCTURE_WAS,
  T_LOAD,
  T_CUTSCENE_BUTTONS,
  T_OBJECTIVES,
  T_REPORT_BUTTONS,
  T_PLAYER_MOTTOS,
  T_PEASANT_FIRST_NAME_MALE,
  T_PEASANT_SURNAMES,
  T_UNIT_ACTIONS,
  T_MARRIAGE,
  T_UNIT_TYPES,
  T_UNIT_COMMENTS,
  T_UNUSED_9,
  T_UNUSED_10,
  T_SIEGE_MAPS,
  T_INSULTS,
  T_PREVIEW_SCREEN,
  T_TUTORIAL,
  T_TUTORIAL_BUTTONS,
  T_MAP_SCREEN,
  T_UNUSED_11,
  T_START_ACT_ONE,
  T_MISSION_ONE_BRIEF,
  T_MISSION_ONE_OBJECTIVES,
  T_MISSION_ONE_HINTS,
  T_MISSION_TWO_NARRATIVE,
  T_MISSION_TWO_BRIEF,
  T_MISSION_TWO_OBJECTIVES,
  T_MISSION_TWO_HINTS,
  T_MISSION_THREE_NARRATIVE,
  T_MISSION_THREE_BRIEF,
  T_MISSION_THREE_OBJECTIVES,
  T_MISSION_THREE_HINTS,
  T_MISSION_FOUR_NARRATIVE,
  T_MISSION_FOUR_BRIEF,
  T_MISSION_FOUR_OBJECTIVES,
  T_MISSION_FOUR_HINTS,
  T_MISSION_FIVE_NARRATIVE,
  T_MISSION_FIVE_BRIEF,
  T_MISSION_FIVE_OBJECTIVES,
  T_MISSION_FIVE_HINTS,
  T_START_ACT_TWO,
  T_MISSION_SIX_BRIEF,
  T_MISSION_SIX_OBJECTIVES,
  T_MISSION_SIX_HINTS,
  T_MISSION_SEVEN_NARRATIVE,
  T_MISSION_SEVEN_BRIEF,
  T_MISSION_SEVEN_OBJECTIVES,
  T_MISSION_SEVEN_HINTS,
  T_MISSION_EIGHT_NARRATIVE,
  T_MISSION_EIGHT_BRIEF,
  T_MISSION_EIGHT_OBJECTIVES,
  T_MISSION_EIGHT_HINTS,
  T_MISSION_NINE_NARRATIVE,
  T_MISSION_NINE_BRIEF,
  T_MISSION_NINE_OBJECTIVES,
  T_MISSION_NINE_HINTS,
  T_MISSION_TEN_NARRATIVE,
  T_MISSION_TEN_BRIEF,
  T_MISSION_TEN_OBJECTIVES,
  T_MISSION_TEN_HINTS,
  T_MISSION_ELEVEN_NARRATIVE,
  T_MISSION_ELEVEN_BRIEF,
  T_MISSION_ELEVEN_OBJECTIVES,
  T_MISSION_ELEVEN_HINTS,
  T_START_ACT_THREE,
  T_MISSION_TWELVE_BRIEF,
  T_MISSION_TWELVE_OBJECTIVES,
  T_MISSION_TWELVE_HINTS,
  T_MISSION_THIRTEEN_NARRATIVE,
  T_MISSION_THIRTEEN_BRIEF,
  T_MISSION_THIRTEEN_OBJECTIVES,
  T_MISSION_THIRTEEN_HINTS,
  T_MISSION_FOURTEEN_NARRATIVE,
  T_MISSION_FOURTEEN_BRIEF,
  T_MISSION_FOURTEEN_OBJECTIVES,
  T_MISSION_FOURTEEN_HINTS,
  T_MISSION_FIFTEEN_NARRATIVE,
  T_MISSION_FIFTEEN_BRIEF,
  T_MISSION_FIFTEEN_OBJECTIVES,
  T_MISSION_FIFTEEN_HINTS,
  T_START_ACT_FOUR,
  T_MISSION_SIXTEEN_BRIEF,
  T_MISSION_SIXTEEN_OBJECTIVES,
  T_MISSION_SIXTEEN_HINTS,
  T_MISSION_SEVENTEEN_NARRATIVE,
  T_MISSION_SEVENTEEN_BRIEF,
  T_MISSION_SEVENTEEN_OBJECTIVES,
  T_MISSION_SEVENTEEN_HINTS,
  T_MISSION_EIGHTEEN_NARRATIVE,
  T_MISSION_EIGHTEEN_BRIEF,
  T_MISSION_EIGHTEEN_OBJECTIVES,
  T_MISSION_EIGHTEEN_HINTS,
  T_MISSION_NINETEEN_NARRATIVE,
  T_MISSION_NINETEEN_BRIEF,
  T_MISSION_NINETEEN_OBJECTIVES,
  T_MISSION_NINETEEN_HINTS,
  T_MISSION_TWENTY_NARRATIVE,
  T_MISSION_TWENTY_BRIEF,
  T_MISSION_TWENTY_OBJECTIVES,
  T_MISSION_TWENTY_HINTS,
  T_MISSION_TWENTY_ONE_NARRATIVE,
  T_MISSION_TWENTY_ONE_BRIEF,
  T_MISSION_TWENTY_ONE_OBJECTIVES,
  T_MISSION_TWENTY_ONE_HINTS,
  T_UNUSED_12,
  T_MISSION_28_FAKE_BRIEF,
  T_MISSION_29_FAKE_BRIEF,
  T_UNUSED_13,
  T_UNUSED_14,
  T_UNUSED_15,
  T_DEMO_MISSIONS_BRIEFS,
  T_HINTS_GENERIC,
  T_ECO_MISSION_ONE_HINTS,
  T_ECO_MISSION_TWO_HINTS,
  T_ECO_MISSION_THREE_HINTS,
  T_ECO_MISSION_FOUR_HINTS,
  T_ECO_MISSION_FIVE_HINTS,
  T_ECO_MISSION_BRIEF,
  T_MISSION_NAMES,
  T_PRE_ATTACK_MESSAGES,
  T_SCENARIO_EDITOR,
  T_TRADER_NAMES,
  T_ACTION_MESSAGES,
  T_CESSPIT,
  T_BURNING_STAKE,
  T_GIBBET,
  T_DUNGEON,
  T_STRETCHING_RACK,
  T_FLOGGING_RACK,
  T_CHOPPING_BLOCK,
  T_DUNKING_STOOL,
  T_CAGED_WAR_DOGS,
  T_STATUE,
  T_SHRINE,
  T_BEEHIVE,
  T_DANCING_BEAR,
  T_POND,
  T_BEAR_CAVE,
  T_UNUSED_16,
  T_UNUSED_17,
  T_STANDALONE_MISSIONS,
  T_UNUSED_18,
  T_UNUSED_19,
  T_UNUSED_20,
  T_UNUSED_21,
  T_UNUSED_22,
  T_UNUSED_23,
  T_UNUSED_24,
  T_UNUSED_25,
  T_UNUSED_26,
  T_MULTIPLAYER_REVISION_CHECK,
  // really dont't care here
};

// ----------------------------------------------------

enum SoundFiles : uint16_t {};

// ----------------------------------------------------
// List of known player names

#define NAME_INDEX_FEMALE 0
#define NAME_INDEX_MALE 44
#define NAME_INDEX_NONSENSE 151
constexpr std::array<const wchar_t*, 154> lut_names = {
    /* Female */
    L"ALLISON",
    L"AMY",
    L"ANDREA",
    L"ANNA",
    L"ANNABELLE",
    L"ANNE",
    L"ANNE MARIE",
    L"AVA",
    L"BETH",
    L"BETTY",
    L"",
    L"BONNIE",
    L"CAMILLE",
    L"CINDY",
    L"COLLETTE",
    L"DARLENE",
    L"DIANNE",
    L"",
    L"ELIZABETH",
    L"ELLEN",
    L"EMMA",
    L"GABRIEL",
    L"HEATHER",
    L"HEIDI",
    L"HELEN",
    L"IVY",
    L"JENNE",
    L"JENNIFER",
    L"JESSICA",
    L"JULIE",
    L"KATE",
    L"KATHLEEN",
    L"MCKANZIE",
    L"MEGAN",
    L"MELISSA",
    L"NICOLE",
    L"PAT",
    L"PATRICIA",
    L"RACHAEL",
    L"RHIAN",
    L"SALLY",
    L"SARAH",
    L"SUSAN",
    L"TRICIA",
    /* Male */
    L"AARON",
    L"AL",
    L"ANDREW",
    L"ANDY",
    L"ANTHONY",
    L"BEN",
    L"BILL",
    L"BRIAN",
    L"BRUCE",
    L"CAS",
    L"CASIMIR",
    L"CHARLES",
    L"CHARLIE",
    L"CHRIS",
    L"CHRISTOFF",
    L"CHRISTOPH",
    L"CLAUDE",
    L"CLIFF",
    L"COLLIN",
    L"DARREN",
    L"DARRIN",
    L"DAVE",
    L"DAVID",
    L"DENBY",
    L"DENNIS",
    L"DON",
    L"DOUG",
    L"EARL",
    L"EMMANUEL",
    L"ERIC",
    L"FAMILY",
    L"FIREFLY",
    L"FRIENDLY",
    L"GEOFF",
    L"GERRY",
    L"GRADY",
    L"GRANT",
    L"GREG",
    L"HARRY",
    L"HEIKO",
    L"IAN",
    L"JACK",
    L"JAMES",
    L"JAMIE",
    L"JASON",
    L"JEFF",
    L"JIMMY",
    L"JOANNA",
    L"JOHN",
    L"JONN",
    L"JOOST",
    L"JORGE",
    L"JOSH",
    L"JULIAN",
    L"KEITH",
    L"KELLY",
    L"KEN",
    L"KEVIN",
    L"KID",
    L"LEE",
    L"LOU",
    L"LOUIE",
    L"LUKE",
    L"MARC",
    L"MARK",
    L"MARKUS",
    L"MATT",
    L"MATTHIAS",
    L"MAURIZIO",
    L"MICHAEL",
    L"MIKE",
    L"NATHAN",
    L"NEAL",
    L"NEIL",
    L"NICK",
    L"OF THE FLIES",
    L"PAOLO",
    L"PATRICK",
    L"PAUL",
    L"PETE",
    L"PETER",
    L"PHIL",
    L"REY",
    L"RICHARD",
    L"ROB",
    L"ROBB",
    L"ROBERT",
    L"ROBIN",
    L"ROLAND",
    L"SAJJAD",
    L"SCOTT",
    L"SEAN",
    L"SETH",
    L"SIMON",
    L"SMITTY",
    L"STEPHANE",
    L"STEVE",
    L"STEVEN",
    L"STU",
    L"STUART",
    L"TERRY",
    L"THIERRY",
    L"THOMAS",
    L"TOM",
    L"TIM",
    L"WAYNE",
    L"YOUENN",
    /* Nonsensical names */
    L"MEGADEATH",
    L"MEGALORD",
    L"SUPER NOODLE",
};
}  // namespace Assets
}  // namespace Sourcehold

