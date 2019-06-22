#include <GUI/MainMenu.h>
#include <Rendering/Font.h>
#include <System/Config.h>
#include <Assets.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

/* All of the menu buttons. TODO: replace double with int */
struct MenuButtonInfo {
    double x;
    double y;
    double w;
    double h;
    const wchar_t *text;
    bool hasGlare;
    uint8_t index; /* Where is the button image in the file? */
    uint8_t glareIndex; /* Where does the glare animation start? */
    uint8_t atlasIndex; /* Which file is the button stored in? */
};
const static MenuButtonInfo lut_buttons[] = {
    { 0.12, 0.67,0.17578125,0.234375, L"Exit Stronghold", false, 68, 0, 0 },
    { 0.423,0.774,0.1337890625,0.098958334, L"Credits", false, 77, 0, 1 },
    { 0.691,0.687,0.17578125,0.234375, L"Tutorial", false, 72, 0, 0 },
    { 0.183,0.276,0.17578125,0.234375, L"Combat-based games", true, 15, 1, 0 },
    { 0.336,0.276,0.17578125,0.234375, L"Economic-based games", true, 32, 18, 0 },
    { 0.492,0.276,0.17578125,0.234375, L"Map Editor", true, 49, 35, 0 },
    { 0.648,0.278,0.17578125,0.234375, L"Load a Saved Game", true, 66, 52, 0 },
    { 0.726,0.511,0.1,0.16, L"Options", false, 88, 0, 0 },
    { 0.183,0.276,0.17578125,0.234375, L"Play the Military Campaign", true, 15, 1, 2 },
    { 0.336,0.276,0.17578125,0.234375, L"Play a Siege", true, 32, 18, 2 },
    { 0.492,0.276,0.17578125,0.234375, L"Play an Invasion", true, 49, 35, 2 },
    { 0.648,0.276,0.17578125,0.234375, L"Play a Multiplayer Game", true, 66, 52, 2 },
    { 0.262,0.276,0.17578125,0.234375, L"Play the Economics Campaign", true, 15, 1, 3 },
    { 0.416,0.276,0.17578125,0.234375, L"Play an Economics Mission", true, 32, 18, 3 },
    { 0.571,0.276,0.17578125,0.234375, L"Free Build", true, 49, 35, 3 },
    { 0.183,0.276,0.17578125,0.234375, L"New Working Map", true, 15, 1, 4 },
    { 0.336,0.276,0.17578125,0.234375, L"New Stand-Alone Mission", true, 32, 18, 4 },
    { 0.492,0.276,0.17578125,0.234375, L"New 'Siege That' Mission", true, 49, 35, 4 },
    { 0.648,0.276,0.17578125,0.234375, L"New Multiplayer Mission", true, 66, 52, 4 },
    { 0.12,0.67,0.17578125,0.234375, L"Back to Main Menu", false, 70, 0, 0 }
};

MainMenu::MainMenu()
{
    edition = GetEdition();

    if (edition == STRONGHOLD_HD) {
        screen.AllocNewTarget(1024, 768);
    }

    aud_chantloop.LoadSong(GetDirectory() / "fx/music/chantloop1.raw", true);

    gm1_icons_main = GetGm1(GetDirectory() / "gm/icons_front_end.gm1").lock();
    gm1_icons_additional = GetGm1(GetDirectory() / "gm/interface_buttons.gm1").lock();
    gm1_icons_combat = GetGm1(GetDirectory() / "gm/icons_front_end_combat.gm1").lock();
    gm1_icons_economic = GetGm1(GetDirectory() / "gm/icons_front_end_economics.gm1").lock();
    gm1_icons_builder = GetGm1(GetDirectory() / "gm/icons_front_end_builder.gm1").lock();

    tgx_bg_main = GetTgx(GetDirectory() / "gfx/frontend_main.tgx").lock();
    tgx_bg_combat = GetTgx(GetDirectory() / "gfx/frontend_combat.tgx").lock();
    tgx_bg_economic = GetTgx(GetDirectory() / "gfx/frontend_economics.tgx").lock();
    tgx_bg_builder = GetTgx(GetDirectory() / "gfx/frontend_builder.tgx").lock();

    aud_greetings.LoadEffect(GetGreetingsSound(), false);
    aud_exit.LoadEffect(GetDirectory() / "fx/speech/General_Quitgame.wav", false);

    mx = (GetWidth() - 1024) / 2;
    my = (GetHeight() - 768) / 2;

    /* Get textures */
    ui_tex.resize(5);
    ui_tex[0] = gm1_icons_main->GetTextureAtlas().lock();
    ui_tex[1] = gm1_icons_additional->GetTextureAtlas().lock();
    ui_tex[2] = gm1_icons_combat->GetTextureAtlas().lock();
    ui_tex[3] = gm1_icons_economic->GetTextureAtlas().lock();
    ui_tex[4] = gm1_icons_builder->GetTextureAtlas().lock();

    /* Allocate buttons */
    if(edition == STRONGHOLD_HD) SetTarget(&screen, mx, my, 1024, 768);
    ui_elems.resize(BUTTON_END);
    for(size_t i = 0; i < BUTTON_END; i++) {
        const MenuButtonInfo *inf = &lut_buttons[i];
        ui_elems[i].Translate(inf->x, inf->y);
        ui_elems[i].Scale(inf->w, inf->h);
        ui_elems[i].SetTexture(ui_tex[inf->atlasIndex].get());
        ui_elems[i].TransparencyCheck(true);
    }
    ResetTarget();
}

MainMenu::~MainMenu()
{

}

UIState MainMenu::EnterMenu()
{
    if(edition == STRONGHOLD_HD) {
        SetTarget(&screen, mx, my, 1024, 768);
    }

    ResetTarget();

    currentState = MAIN_MENU;

    aud_chantloop.Play();
    aud_greetings.Play();

    while (Running()) {
        ClearDisplay();

        if(edition == STRONGHOLD_HD) {
            RenderMenuBorder();
            SetTarget(&screen, mx, my, 1024, 768);
        }

        /* Render the current menu on top of the background */
        HideAll();

        int buttonEnd = BUTTON_END, buttonStart = MAIN_EXIT;
        switch(currentState) {
        case MAIN_MENU: {
            Render(*tgx_bg_main);
            buttonEnd = COMBAT_CAMPAIGN;
        }
        break;
        case COMBAT_MENU: {
            Render(*tgx_bg_combat);
            buttonEnd = ECO_CAMPAIGN;
            buttonStart = COMBAT_CAMPAIGN;
        }
        break;
        case ECONOMICS_MENU: {
            Render(*tgx_bg_economic);
            buttonEnd = BUILDER_WORKING_MAP;
            buttonStart = ECO_CAMPAIGN;
        }
        break;
        case BUILDER_MENU: {
            Render(*tgx_bg_builder);
            buttonStart = BUILDER_WORKING_MAP;
        }
        break;
        default:
            break;
        }

        selected = BUTTON_END;
        for(int i = buttonStart; i < buttonEnd; i++) {
            ui_elems[i].Show();
            ui_elems[i].Render(
            [&]() -> SDL_Rect {
                MenuButtonInfo inf = lut_buttons[i];
                auto tex = ui_tex[inf.atlasIndex];

                if(ui_elems[i].IsClicked()) selected = (MenuButton)i;
                {
                    if (ui_elems[i].IsMouseOver()) {
                        RenderMenuText(inf.text);
                        return tex->Get(inf.index + 1);
                    }
                    else {
                        if(inf.hasGlare) {
                            return tex->Get(inf.glareIndex + (glareCounter % 14));
                        }
                        else {
                            return tex->Get(inf.index);
                        }
                    }
                }
            });
        }

        if(currentState != MAIN_MENU) {
            RenderBackToMain();
        }

        /* Check buttons */
        switch(selected) {
        case MAIN_EXIT: {
            // TODO: exit prompt
            aud_chantloop.Stop();
            return EXIT_GAME;
        }
        break;
        case MAIN_COMBAT: {
            currentState = COMBAT_MENU;
        }
        break;
        case MAIN_ECONOMIC: {
            currentState = ECONOMICS_MENU;
        }
        break;
        case MAIN_BUILDER: {
            currentState = BUILDER_MENU;
        }
        break;
        case MAIN_LOAD: {
            currentState = LOAD_SAVED_MENU;
        }
        break;
        case MAIN_TUTORIAL: {
            currentState = TUTORIAL;
        }
        break;
        case MAIN_FIREFLY: {
            currentState = CREDITS;
        }
        break;
        case BACK_TO_MAIN: {
            currentState = MAIN_MENU;
        }
        break;
        case MAIN_SETTINGS: {
            currentState = SETTINGS_MENU;
        }
        break;
        default:
            break;
        }

        RenderText(L"V" SOURCEHOLD_VERSION_STRING, 4, 4, 1, FONT_SMALL);

        ResetTarget();
        
        if(edition == STRONGHOLD_HD) {
            Render(screen, mx, my);
        }

        /* Reset target first, then play credits and leave again */
        if(currentState == CREDITS) {
            aud_chantloop.Stop();
            /* Start credits loop */
            cred.Play(false, true, true);
            aud_chantloop.SetGain(1.0);
            aud_chantloop.Resume();

            currentState = MAIN_MENU;
        }

        aud_greetings.Update();

        glareCounter = (int)(GetTime() * 10.0);

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

void MainMenu::RenderBackToMain()
{
    ui_elems[BACK_TO_MAIN].Show();
    ui_elems[BACK_TO_MAIN].Render(
    [&]() -> SDL_Rect {
        MenuButtonInfo inf = lut_buttons[BACK_TO_MAIN];
        auto tex = ui_tex[inf.atlasIndex];

        if(ui_elems[BACK_TO_MAIN].IsClicked()) selected = BACK_TO_MAIN;
        if (ui_elems[BACK_TO_MAIN].IsMouseOver())
        {
            RenderMenuText(inf.text);
            return tex->Get(inf.index + 1);
        }
        else {
            return tex->Get(inf.index);
        }
    });
}

void MainMenu::HideAll()
{
    for(auto & e : ui_elems) {
        e.Hide();
    }
}
