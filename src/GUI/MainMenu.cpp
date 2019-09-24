#include <map>
#include <boost/assign.hpp>

#include "GUI/MainMenu.h"
#include "Rendering/Font.h"
#include "System/Config.h"
#include "Assets.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

/* All of the menu buttons. TODO: replace double with int */
struct MenuButtonInfo {
    double x;
    double y;
    TextSection sec;
    uint16_t textIndex;
    bool hasGlare;
    bool hasText;
    uint8_t index;
    uint8_t glareIndex;
    uint8_t glareOrder;
    uint8_t atlasIndex;
};

const static MenuButtonInfo lut_buttons[] = {
    // MAIN_MENU //
    {  0.12, 0.67, T_MAIN_MENU,      5, false, true, 68,  0, 0, 0 },
    { 0.423,0.774, T_BUILDINGS,    301, false, true, 77,  0, 0, 1 },
    { 0.691,0.687, T_BUILDINGS,    286, false, true, 72,  0, 0, 0 },
    { 0.182,0.276, T_BUILDINGS,    226, true,  true, 15,  1, 0, 0 },
    { 0.337,0.276, T_BUILDINGS,    227, true,  true, 32, 18, 1, 0 },
    { 0.492,0.276, T_BUILDINGS,    228, true,  true, 49, 35, 2, 0 },
    { 0.647,0.278, T_BUILDINGS,    229, true,  true, 66, 52, 3, 0 },
    { 0.732, 0.54, T_GAME_OPTIONS,  24, false, true, 88,  0, 0, 1 },
    // COMBAT_MENU //
    { 0.183,0.276, T_BUILDINGS, 258, true,  true,  15,  1, 0, 2 },
    { 0.336,0.276, T_BUILDINGS, 260, true,  true,  32, 18, 1, 2 },
    { 0.492,0.276, T_BUILDINGS, 261, true,  true,  49, 35, 2, 2 },
    { 0.648,0.276, T_BUILDINGS, 259, true,  true,  66, 52, 3, 2 },
    {   0.1,  0.7, T_BUILDINGS, 270, false, true,  70,  0, 0, 0 },
    {   0.1,  0.7, T_BUILDINGS,   0, false, false, 68,  0, 0, 2 }, // back
    {  0.73,  0.7, T_BUILDINGS,   0, false, false, 70,  0, 0, 2 }, // next
    // ECONOMICS_MENU //
    { 0.261,0.276, T_BUILDINGS, 262,  true,  true, 15,  1, 0, 3 },
    { 0.417,0.276, T_BUILDINGS, 263,  true,  true, 32, 18, 1, 3 },
    { 0.571,0.276, T_BUILDINGS, 264,  true,  true, 49, 35, 2, 3 },
    {   0.1,  0.7, T_BUILDINGS, 270, false,  true, 70,  0, 0, 0 },
    {   0.1,  0.7, T_BUILDINGS,   0, false, false, 51,  0, 0, 3 }, // back
    {  0.73,  0.7, T_BUILDINGS,   0, false, false, 53,  0, 0, 3 }, // next
    // BUILDER_MENU //
    { 0.182,0.276, T_BUILDINGS, 266,  true, true, 15,  1, 0, 4 },
    { 0.336,0.276, T_BUILDINGS, 267,  true, true, 32, 18, 1, 4 },
    { 0.492,0.276, T_BUILDINGS, 268,  true, true, 49, 35, 2, 4 },
    { 0.648,0.276, T_BUILDINGS, 269,  true, true, 66, 52, 3, 4 },
    {   0.1,  0.7, T_BUILDINGS, 270, false, true, 70,  0, 0, 4 }
};

/* Connects a button to a UIState */
const static std::map<MenuButton, UIState> actions = boost::assign::map_list_of
    // MAIN_MENU //
    (MAIN_EXIT, EXIT_GAME)
    (MAIN_FIREFLY, CREDITS)
    (MAIN_TUTORIAL, TUTORIAL)
    (MAIN_COMBAT, COMBAT_MENU)
    (MAIN_ECONOMIC, ECONOMICS_MENU)
    (MAIN_BUILDER, BUILDER_MENU)
    (MAIN_LOAD, LOAD_SAVED_MENU)
    (MAIN_SETTINGS, SETTINGS_MENU)
    // COMBAT_MENU //
    (COMBAT_CAMPAIGN, MILITARY_CAMPAIGN_MENU)
    (COMBAT_SIEGE, SIEGE_MENU)
    (COMBAT_INVASION, INVASION_MENU)
    (COMBAT_MULTIPLAYER, MULTIPLAYER_MENU)
    (COMBAT_BACK_TO_MAIN, MAIN_MENU)
    (COMBAT_CAMPAIGN_BACK, COMBAT_MENU)
    (COMBAT_CAMPAIGN_NEXT, MILITARY_CAMPAIGN_MISSION)
    // ECONOMICS_MENU //
    (ECO_CAMPAIGN, ECONOMICS_CAMPAIGN_MENU)
    (ECO_MISSION, ECONOMICS_MISSION_MENU)
    (ECO_FREEBUILD, FREE_BUILD_MENU)
    (ECO_BACK_TO_MAIN, MAIN_MENU)
    (ECO_CAMPAIGN_BACK, ECONOMICS_MENU)
    (ECO_CAMPAIGN_NEXT, ECONOMICS_CAMPAIGN_MENU) // todo
    // BUILDER_MENU //
    (BUILDER_WORKING_MAP, WORKING_MAP_MENU)
    (BUILDER_STANDALONE, STAND_ALONE_MISSION_MENU)
    (BUILDER_SIEGE, SIEGE_THAT_MENU)
    (BUILDER_MULTIPLAYER, MULTIPLAYER_MAP_MENU)
    (BUILDER_BACK_TO_MAIN, MAIN_MENU);

MainMenu::MainMenu()
{
    edition = GetEdition();

    mx = (GetWidth() - 1024) / 2;
    my = (GetHeight() - 768) / 2;

    if (edition == STRONGHOLD_HD) {
        screen.AllocNewTarget(1024, 768);
        SetTarget(&screen, Rect<int>{ mx, my, 1024, 768 });
    }

    bool success = true;
    success &= aud_chantloop.Load(GetDirectory() / "fx/music/chantloop1.raw", true);
    success &= aud_greetings.Load(GetGreetingsSound(), false);
    success &= aud_exit.Load(GetDirectory() / "fx/speech/General_Quitgame.wav", false);
    if(!success) {
        Logger::error(Subsystem::GUI) << "Unable to initialize main menu: failed to load SFX!" << std::endl;
    }

    gm1_icons_main =        GetGm1("gm/icons_front_end.gm1");
    gm1_icons_additional =  GetGm1("gm/interface_buttons.gm1");
    gm1_icons_combat =      GetGm1("gm/icons_front_end_combat.gm1");
    gm1_icons_economic =    GetGm1("gm/icons_front_end_economics.gm1");
    gm1_icons_builder =     GetGm1("gm/icons_front_end_builder.gm1");

    tgx_bg_main =           GetTgx("gfx/frontend_main.tgx");
    tgx_bg_main2 =          GetTgx("gfx/frontend_main2.tgx");
    tgx_bg_combat =         GetTgx("gfx/frontend_combat.tgx");
    tgx_bg_combat2 =        GetTgx("gfx/frontend_combat2.tgx");
    tgx_bg_economic =       GetTgx("gfx/frontend_economics.tgx");
    tgx_bg_economic2 =      GetTgx("gfx/frontend_economics2.tgx");
    tgx_bg_builder =        GetTgx("gfx/frontend_builder.tgx");

    /* Get textures */
    ui_tex.resize(5);
    ui_tex[0] = gm1_icons_main->GetTextureAtlas();
    ui_tex[1] = gm1_icons_additional->GetTextureAtlas();
    ui_tex[2] = gm1_icons_combat->GetTextureAtlas();
    ui_tex[3] = gm1_icons_economic->GetTextureAtlas();
    ui_tex[4] = gm1_icons_builder->GetTextureAtlas();

    dlg[0].Init(DialogType::QUIT);
    dlg[1].Init(DialogType::LOAD);
    dlg[2].Init(DialogType::COMBAT_MENU);
    dlg[3].Init(DialogType::SIEGE_MENU);
    dlg[4].Init(DialogType::ECO_MENU);
    dlg[5].Init(DialogType::SETTINGS);

    /* Allocate buttons */
    ui_elems.resize(BUTTON_END);
    for(size_t i = 0; i < BUTTON_END; i++) {
        const MenuButtonInfo *inf = &lut_buttons[i];
        auto atlas = ui_tex[inf->atlasIndex];
        auto rect = atlas->Get(inf->index);

        ui_elems[i].Translate(inf->x, inf->y);
        ui_elems[i].Scale(rect.w, rect.w);
        ui_elems[i].SetTexture(atlas.get());
    }
    ResetTarget();
}

MainMenu::~MainMenu()
{
}

UIState MainMenu::EnterMenu()
{
    UIState currentState = MAIN_MENU;

    aud_chantloop.Play();
    //aud_greetings.Play();

    while (Running()) {
        ClearDisplay();

        if(edition == STRONGHOLD_HD) {
            RenderMenuBorder();
            SetTarget(&screen, Rect<int>{ mx, my, 1024, 768 });
        }

        /* Render the current menu on top of the background */
        HideAll();

        /**
         * Handle the current UIState.
         * buttonEnd and buttonStart define the range of
         * buttons to be rendered for the menu page.
         */
        MenuButton selected = BUTTON_END;
        int buttonEnd = BUTTON_END, buttonStart = MAIN_EXIT;
        switch(currentState) {
        default:
        case MAIN_MENU: {
            Render(*tgx_bg_main);
            buttonEnd = COMBAT_CAMPAIGN;
        } break;
        case COMBAT_MENU: {
            Render(*tgx_bg_combat);
            buttonEnd = COMBAT_BACK_TO_MAIN+1;
            buttonStart = COMBAT_CAMPAIGN;
        } break;
        case MILITARY_CAMPAIGN_MENU: {
            Render(*tgx_bg_combat2);

            DialogResult res = dlg[2].Render();
            if(res == LOAD) {
                // Double-click on mission -> start mission
                return MILITARY_CAMPAIGN_MISSION;
            }

            buttonEnd = COMBAT_CAMPAIGN_NEXT+1;
            buttonStart = COMBAT_CAMPAIGN_BACK;
        } break;
        case MILITARY_CAMPAIGN_MISSION: {
            // TODO
            if(dlg[2].GetSelectedIndex() >= 0) {
                aud_chantloop.Stop();
                ResetTarget();
                return MILITARY_CAMPAIGN_MISSION;
            }
            else currentState = MILITARY_CAMPAIGN_MENU;
        }
        case SIEGE_MENU: {
            Render(*tgx_bg_combat2);

            DialogResult res = dlg[3].Render();
            buttonStart = buttonEnd = MAIN_EXIT;
        } break;
        case ECONOMICS_MENU: {
            Render(*tgx_bg_economic);
            buttonEnd = ECO_BACK_TO_MAIN+1;
            buttonStart = ECO_CAMPAIGN;
        } break;
        case ECONOMICS_CAMPAIGN_MENU: {
            Render(*tgx_bg_economic2);

            DialogResult res = dlg[4].Render();
            if(res == LOAD) {
                return ECONOMICS_CAMPAIGN_MISSION;
            }

            buttonEnd = ECO_CAMPAIGN_NEXT+1;
            buttonStart = ECO_CAMPAIGN_BACK;
        } break;
        case BUILDER_MENU: {
            Render(*tgx_bg_builder);
            buttonEnd = BUILDER_BACK_TO_MAIN+1;
            buttonStart = BUILDER_WORKING_MAP;
        } break;
        case LOAD_SAVED_MENU: {
            Render(*tgx_bg_main2);
            DialogResult res = dlg[1].Render();
            if(res == LOAD) {
                // TODO
            } else if(res == BACK) {
                currentState = MAIN_MENU;
            }
            buttonStart = buttonEnd = MAIN_EXIT;
        } break;
        case EXIT_GAME: {
            Render(*tgx_bg_main2);
            DialogResult res = dlg[0].Render();
            if(res == QUIT) {
                aud_chantloop.Stop();
                return EXIT_GAME;
            } else if(res == BACK) {
                currentState = MAIN_MENU;
            }
            buttonStart = buttonEnd = MAIN_EXIT;
        } break;
        case SETTINGS_MENU: {
            Render(*tgx_bg_main2);
            DialogResult res = dlg[5].Render();
            if(res == BACK) {
                currentState = MAIN_MENU;
            }
            buttonStart = buttonEnd = MAIN_EXIT;
        } break;
        }

        int glareTicks = (int)(GetTime() * 10.0);
        glareCounter = (glareTicks / 14) % 4;

        for(int i = buttonStart; i < buttonEnd; i++) {
            ui_elems[i].Show();
            ui_elems[i].Render(
            [&]() -> SDL_Rect {
                MenuButtonInfo inf = lut_buttons[i];
                auto tex = ui_tex[inf.atlasIndex];

                if(ui_elems[i].IsClicked()) selected = (MenuButton)i;
                if (ui_elems[i].IsMouseOver()) {
                    if(inf.hasText) RenderMenuText(GetString(inf.sec, inf.textIndex));
                    return tex->Get(inf.index + 1);
                } else {
                    if(inf.hasGlare && inf.glareOrder == glareCounter) {
                        return tex->Get(inf.glareIndex + (glareTicks % 14));
                    }
                    else {
                        return tex->Get(inf.index);
                    }
                }
            });
        }

        if(selected != BUTTON_END) {
            try {
                currentState = actions.at(selected);
            } catch(std::out_of_range & ex) {
                currentState = MAIN_MENU;
            }
        }

        RenderText(L"V." SOURCEHOLD_VERSION_STRING, 6, 4, FONT_LARGE, false, 0.5);
        ResetTarget();

        if(edition == STRONGHOLD_HD) {
            Render(screen, mx, my);
        }

        //aud_greetings.Update();

        /* Reset target first, then play credits and leave again */
        if(currentState == CREDITS) {
            aud_chantloop.Stop();
            /* Start credits loop */
            cred.Play(false, true, true);
            aud_chantloop.SetGain(1.0);
            aud_chantloop.Resume();

            currentState = MAIN_MENU;
        }

        FlushDisplay();
        SyncDisplay();
    }

    aud_chantloop.Stop();
    return EXIT_GAME;
}

boost::filesystem::path MainMenu::GetGreetingsSound()
{
    boost::filesystem::path snd = GetDirectory() / "fx/speech/";

    int index = GetUsernameIndex();
    if(index == -1) {
        snd /= "General_Startgame.wav";
    }
    else {
        snd /= "name" + std::to_string(index + 1) + ".wav";
    }

    return snd;
}

void MainMenu::HideAll()
{
    for(auto & e : ui_elems) {
        e.Hide();
    }
}


