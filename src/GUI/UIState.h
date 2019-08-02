#pragma once

#include <cinttypes>

namespace Sourcehold {
    namespace GUI {
        enum UIState : uint8_t {
            INTRO_SEQUENCE,
            MAIN_MENU,
            COMBAT_MENU,
            ECONOMICS_MENU,
            BUILDER_MENU,
            LOAD_SAVED_MENU,
            MILITARY_CAMPAIGN_MENU,
            MILITARY_CAMPAIGN_MISSION,
            SIEGE_MENU,
            INVASION_MENU,
            MULTIPLAYER_MENU,
            ECONOMICS_CAMPAIGN_MENU,
            ECONOMICS_MISSION_MENU,
            ECONOMICS_CAMPAIGN_MISSION,
            FREE_BUILD_MENU,
            WORKING_MAP_MENU,
            STAND_ALONE_MISSION_MENU,
            SIEGE_THAT_MENU,
            MULTIPLAYER_MAP_MENU,
            SETTINGS_MENU,
            TUTORIAL,
            CREDITS,
            EXIT_GAME
        };
    }
}
