#include <map>
#include <memory>

#include "GUI/MainMenu.h"
#include "GUI/NarrScreen.h"

#include "Rendering/Font.h"
#include "System/Config.h"
#include "Assets.h"
#include "World.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

struct MenuButtonInfo {
  int x, y;
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
    {123, 515, T_MAIN_MENU, 5, false, true, 68, 0, 0, 0},
    {433, 594, T_BUILDINGS, 301, false, true, 77, 0, 0, 1},
    {708, 528, T_BUILDINGS, 286, false, true, 72, 0, 0, 0},
    {186, 212, T_BUILDINGS, 226, true, true, 15, 1, 0, 0},
    {345, 212, T_BUILDINGS, 227, true, true, 32, 18, 1, 0},
    {504, 212, T_BUILDINGS, 228, true, true, 49, 35, 2, 0},
    {663, 212, T_BUILDINGS, 229, true, true, 66, 52, 3, 0},
    {750, 415, T_GAME_OPTIONS, 24, false, true, 88, 0, 0, 1},
    // COMBAT_MENU //
    {187, 212, T_BUILDINGS, 258, true, true, 15, 1, 0, 2},
    {344, 212, T_BUILDINGS, 260, true, true, 32, 18, 1, 2},
    {504, 212, T_BUILDINGS, 261, true, true, 49, 35, 2, 2},
    {664, 212, T_BUILDINGS, 259, true, true, 66, 52, 3, 2},
    {102, 534, T_BUILDINGS, 270, false, true, 70, 0, 0, 0},
    {102, 534, T_BUILDINGS, 0, false, false, 68, 0, 0, 2},  // back
    {748, 534, T_BUILDINGS, 0, false, false, 70, 0, 0, 2},  // next
    // ECONOMICS_MENU //
    {267, 212, T_BUILDINGS, 262, true, true, 15, 1, 0, 3},
    {427, 212, T_BUILDINGS, 263, true, true, 32, 18, 1, 3},
    {585, 212, T_BUILDINGS, 264, true, true, 49, 35, 2, 3},
    {102, 534, T_BUILDINGS, 270, false, true, 70, 0, 0, 0},
    {102, 534, T_BUILDINGS, 0, false, false, 51, 0, 0, 3},  // back
    {748, 534, T_BUILDINGS, 0, false, false, 53, 0, 0, 3},  // next
    // BUILDER_MENU //
    {186, 212, T_BUILDINGS, 266, true, true, 15, 1, 0, 4},
    {344, 212, T_BUILDINGS, 267, true, true, 32, 18, 1, 4},
    {504, 212, T_BUILDINGS, 268, true, true, 49, 35, 2, 4},
    {663, 212, T_BUILDINGS, 269, true, true, 66, 52, 3, 4},
    {102, 534, T_BUILDINGS, 270, false, true, 70, 0, 0, 4}};

/* Connects a button to a UIState */
const static std::map<MenuButton, UIState> actions{
    // MAIN_MENU //
    {MAIN_EXIT, EXIT_GAME},
    {MAIN_FIREFLY, CREDITS},
    {MAIN_TUTORIAL, TUTORIAL},
    {MAIN_COMBAT, COMBAT_MENU},
    {MAIN_ECONOMIC, ECONOMICS_MENU},
    {MAIN_BUILDER, BUILDER_MENU},
    {MAIN_LOAD, LOAD_SAVED_MENU},
    {MAIN_SETTINGS, SETTINGS_MENU},
    // COMBAT_MENU //
    {COMBAT_CAMPAIGN, MILITARY_CAMPAIGN_MENU},
    {COMBAT_SIEGE, SIEGE_MENU},
    {COMBAT_INVASION, INVASION_MENU},
    {COMBAT_MULTIPLAYER, MULTIPLAYER_MENU},
    {COMBAT_BACK_TO_MAIN, MAIN_MENU},
    {COMBAT_CAMPAIGN_BACK, COMBAT_MENU},
    {COMBAT_CAMPAIGN_NEXT, MILITARY_CAMPAIGN_MISSION},
    // ECONOMICS_MENU //
    {ECO_CAMPAIGN, ECONOMICS_CAMPAIGN_MENU},
    {ECO_MISSION, ECONOMICS_MISSION_MENU},
    {ECO_FREEBUILD, FREE_BUILD_MENU},
    {ECO_BACK_TO_MAIN, MAIN_MENU},
    {ECO_CAMPAIGN_BACK, ECONOMICS_MENU},
    {ECO_CAMPAIGN_NEXT, ECONOMICS_CAMPAIGN_MENU},  // todo
    // BUILDER_MENU //
    {BUILDER_WORKING_MAP, WORKING_MAP_MENU},
    {BUILDER_STANDALONE, STAND_ALONE_MISSION_MENU},
    {BUILDER_SIEGE, SIEGE_THAT_MENU},
    {BUILDER_MULTIPLAYER, MULTIPLAYER_MAP_MENU},
    {BUILDER_BACK_TO_MAIN, MAIN_MENU}};

MainMenu::MainMenu() {
  int mx = (GetWidth() - 1024) / 2;
  int my = (GetHeight() - 768) / 2;

  bool success = true;
  success &=
      aud_chantloop.Load(GetDirectory() / "fx/music/chantloop1.raw", true);
  success &= aud_greetings.Load(GetGreetingsSound(), false);
  success &=
      aud_exit.Load(GetDirectory() / "fx/speech/General_Quitgame.wav", false);
  if (!success) {
    Logger::error(Subsystem::GUI)
        << "Unable to initialize main menu: failed to load SFX!" << std::endl;
  }

  auto gm1_icons_main = GetGm1("gm/icons_front_end.gm1");
  auto gm1_icons_additional = GetGm1("gm/interface_buttons.gm1");
  auto gm1_icons_combat = GetGm1("gm/icons_front_end_combat.gm1");
  auto gm1_icons_economic = GetGm1("gm/icons_front_end_economics.gm1");
  auto gm1_icons_builder = GetGm1("gm/icons_front_end_builder.gm1");

  bg_main[0] = GetTgx("gfx/frontend_main.tgx");
  bg_main[1] = GetTgx("gfx/frontend_main2.tgx");
  bg_combat[0] = GetTgx("gfx/frontend_combat.tgx");
  bg_combat[1] = GetTgx("gfx/frontend_combat2.tgx");
  bg_eco[0] = GetTgx("gfx/frontend_economics.tgx");
  bg_eco[1] = GetTgx("gfx/frontend_economics2.tgx");
  bg_builder[0] = GetTgx("gfx/frontend_builder.tgx");
  bg_builder[1] = GetTgx("gfx/frontend_builder2.tgx");

  /* Get textures */
  ui_tex.resize(5);
  ui_tex[0] = gm1_icons_main->GetTextureAtlas();
  ui_tex[1] = gm1_icons_additional->GetTextureAtlas();
  ui_tex[2] = gm1_icons_combat->GetTextureAtlas();
  ui_tex[3] = gm1_icons_economic->GetTextureAtlas();
  ui_tex[4] = gm1_icons_builder->GetTextureAtlas();

  /* Allocate buttons */
  ui_elems.resize(BUTTON_END);
  for (size_t i = 0; i < BUTTON_END; i++) {
    const MenuButtonInfo& inf = lut_buttons[i];
    auto atlas = ui_tex[inf.atlasIndex];
    auto inactive = atlas->Get(inf.index + 0);
    auto active = atlas->Get(inf.index + 1);

    ui_elems[i].Transform({mx + inf.x, my + inf.y, active.w, active.h});
    ui_elems[i].SetTexture(atlas.get());
    ui_elems[i].SetActiveRect(active);
    ui_elems[i].SetInactiveRect(inactive);
    ui_elems[i].SetID(i);

    ui_elems[i].onEvent = [&](size_t id, Mouse& m) {
      if (id >= BUTTON_END || id < 0)
        return;
      if (m.type == BUTTONDOWN && m.LmbDown()) {
        MenuButton selected = static_cast<MenuButton>(id);
        if (selected != BUTTON_END) {
          try {
            currentState = actions.at(selected);
          }
          catch (std::out_of_range& ex) {
            currentState = MAIN_MENU;
          }
        }
      }
    };
  }
}

MainMenu::~MainMenu() {
}

UIState MainMenu::EnterMenu() {
  currentState = MAIN_MENU;

  aud_chantloop.Play();
  // aud_greetings.Play();

  auto quitDlg = QuitDialog(), loadDlg = LoadDialog(),
       combatMenuDlg = CombatMenuDialog();
  quitDlg->onExit = [&]() {
    currentState = MAIN_MENU;
  };

  while (Running()) {
    ClearDisplay();

    int mx = (GetWidth() - 1024) / 2;
    int my = (GetHeight() - 768) / 2;

    RenderMenuBorder();

    /* Handle the current menu state */
    HideButtons();
    switch (currentState) {
      default:
      case MAIN_MENU: {
        Render(*bg_main[0], mx, my);
        RenderButtons(MAIN_EXIT, MAIN_SETTINGS);
      } break;
      case COMBAT_MENU: {
        Render(*bg_combat[0], mx, my);
        RenderButtons(COMBAT_CAMPAIGN, COMBAT_BACK_TO_MAIN);
      } break;
      case MILITARY_CAMPAIGN_MENU: {
        Render(*bg_combat[1], mx, my);
        RenderButtons(COMBAT_CAMPAIGN_BACK, COMBAT_CAMPAIGN_NEXT);

        combatMenuDlg->Update(Dialog::CENTRE, 0, -60);
      } break;
      case MILITARY_CAMPAIGN_MISSION: {
        // Start the selected campaign mission (TODO) //
        aud_chantloop.Stop();

        int index = 0;
        std::make_unique<NarrScreen>(index + 1)->Begin();

        Song music(GetDirectory() / "fx/music/the maidenA.raw", true);
        music.Play();

        World* world = new World();
        world->LoadFromDisk(GetDirectory() / "maps/mission1.map");
        currentState = world->Play();

        music.Stop();
        aud_chantloop.Resume();
      } break;
      case SIEGE_MENU: {
        Render(*bg_combat[1], mx, my);
      } break;
      case ECONOMICS_MENU: {
        Render(*bg_eco[0], mx, my);
        RenderButtons(ECO_CAMPAIGN, ECO_BACK_TO_MAIN);
      } break;
      case ECONOMICS_CAMPAIGN_MENU: {
        Render(*bg_eco[1], mx, my);
        RenderButtons(ECO_CAMPAIGN_BACK, ECO_CAMPAIGN_NEXT);
      } break;
      case BUILDER_MENU: {
        Render(*bg_builder[0], mx, my);
        RenderButtons(BUILDER_WORKING_MAP, BUILDER_BACK_TO_MAIN);
      } break;
      case LOAD_SAVED_MENU: {
        Render(*bg_main[1], mx, my);
        loadDlg->Update(Dialog::CENTRE, 0, 0);
      } break;
      case EXIT_GAME: {
        Render(*bg_main[1], mx, my);
        quitDlg->Update(Dialog::CENTRE, 0, 0);
      } break;
      case SETTINGS_MENU: {
        Render(*bg_main[1], mx, my);
      } break;
    }

    RenderText(L"V." SOURCEHOLD_VERSION_STRING, mx + 6, my + 4, FONT_LARGE,
               false, 0.5);

    /*if (GetResolution() == RESOLUTION_800x600 // always scale for 800x600
#if SCALE_MAIN_MENU == 1
        || GetEdition() == STRONGHOLD_CLASSIC // scale for classic edition too
(optional) #endif ) { Render(screen);
    }
    else {
        Render(screen, mx, my);
    }*/

    // aud_greetings.Update();

    /* Reset target first, then play credits and leave again */
    if (currentState == CREDITS) {
      aud_chantloop.Stop();
      /* Start credits loop */
      cred.Play(false, true, true);
      aud_chantloop.SetGain(1.0);
      aud_chantloop.Resume();

      currentState = MAIN_MENU;
    }

    FlushDisplay();
    SDL_Delay(1);
  }

  aud_chantloop.Stop();
  return EXIT_GAME;
}

ghc::filesystem::path MainMenu::GetGreetingsSound() {
  ghc::filesystem::path snd = GetDirectory() / "fx/speech/";

  int index = GetUsernameIndex();
  if (index == -1) {
    snd /= "General_Startgame.wav";
  }
  else {
    snd /= "name" + std::to_string(index + 1) + ".wav";
  }

  return snd;
}

void MainMenu::RenderButtons(MenuButton start, MenuButton end) {
  if (start >= end)
    return;

  int glareTicks = (int)(GetTime() * 10.0);
  int glareCounter = (glareTicks / 14) % 4;

  for (int i = start; i <= end; i++) {
    const MenuButtonInfo& inf = lut_buttons[i];
    auto tex = ui_tex[inf.atlasIndex];

    // Animate the inactive graphic //
    SDL_Rect inactive = tex->Get(inf.index);
    if (inf.hasGlare && inf.glareOrder == glareCounter) {
      ui_elems[i].SetInactiveRect(tex->Get(inf.glareIndex + (glareTicks % 14)));
    }
    else {
      ui_elems[i].SetInactiveRect(inactive);
    }

    // Update position //
    int mx = (GetWidth() - 1024) / 2;
    int my = (GetHeight() - 768) / 2;

    ui_elems[i].visible = true;

    ui_elems[i].Transform({mx + inf.x, my + inf.y, inactive.w, inactive.h});
    ui_elems[i].Render();
  }
}

void MainMenu::HideButtons() {
  // hack
  for (int i = 0; i < BUTTON_END; i++) {
    ui_elems[i].visible = false;
  }
}
