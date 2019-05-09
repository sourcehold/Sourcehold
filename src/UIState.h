#pragma once


namespace Sourcehold
{
    namespace Game
    {
        enum UIState : uint16_t {
            INTRO_SEQUENCE = 0,
            MAIN_MENU = 1,
            COMBAT_MENU = 2,
            ECONOMICS_MENU = 3,
            BUILDER_MENU =  4,
            LOAD_SAVED_MENU = 5,
            MILITARY_CAMPAIGN_MENU = 6,
            SIEGE_MENU = 7,
            INVASION_MENU = 8,
            MULTIPLAYER_MENU = 9,
            ECONOMICS_CAMPAIGN_MENU = 10,
            ECONOMICS_MISSION_MENU = 11,
            FREE_BUILD_MENU = 12,
            WORKING_MAP_MENU = 13,
            STAND_ALONE_MISSION_MENU = 14,
            SIEGE_THAT_MENU = 15,
            MULTIPLAYER_MAP_MENU = 16,
            SETTINGS_MENU = 17,
            EXIT_GAME = 18
        };
    }
}
