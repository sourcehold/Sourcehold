#include "GUI/Ingame.h"
#include "System/Config.h"

using namespace Sourcehold;
using namespace GUI;

IngameGUI::IngameGUI() : EventConsumer<Keyboard>(), EventConsumer<Mouse>() {
  LoadMenuAssets();

  /* Init the buttons */
  auto atlas = gm1_floats->GetTextureAtlas();
  ui_compass.SetTexture(atlas.get());
  ui_hide.SetTexture(atlas.get());
  ui_magnify.SetTexture(atlas.get());
  ui_lower.SetTexture(atlas.get());

  atlas = gm1_icons->GetTextureAtlas();
  SDL_Rect rect = atlas->Get(25);
  ui_disk.Transform(Rect<int>(rect.x, rect.y, rect.w, rect.h));
  ui_disk.SetTexture(atlas.get());

  rect = atlas->Get(27);
  ui_info.Transform(Rect<int>(rect.x, rect.y, rect.w, rect.h));
  ui_info.SetTexture(atlas.get());

  rect = atlas->Get(29);
  ui_delete.Transform(Rect<int>(rect.x, rect.y, rect.w, rect.h));
  ui_delete.SetTexture(atlas.get());

  rect = atlas->Get(68);
  ui_revert.Transform(Rect<int>(rect.x, rect.y, rect.w, rect.h));
  ui_revert.SetTexture(atlas.get());
}

IngameGUI::~IngameGUI() {
}

bool IngameGUI::Render() {
  if (rmbHolding) {
    RenderQuickMenu();
  }

  if (menubarShown) {
    RenderMenubar();
  }

  return true;
}

int IngameGUI::GetMenubarHeight() {
  return menubarShown ? 200 : 0;
}

static const char* _res_to_edge[][2] = {
    {"edge1024", "edge_military_1024"}, {"edge1280", "edge_military_1280"},
    {"edge1280", "edge_military_1280"}, {"edge1360", "edge_military_1360"},
    {"edge1366", "edge_military_1366"}, {"edge1440", "edge_military_1440"},
    {"edge1600", "edge_military_1600"}, {"edge1600", "edge_military_1600"},
    {"edge1680", "edge_military_1680"}, {"edge1920", "edge_military_1920"}};

void IngameGUI::LoadMenuAssets() {
  Resolution res = GetResolution();
  if (res != RESOLUTION_800x600 && res != RESOLUTION_DYNAMIC) {
    std::string base(_res_to_edge[res][0]);

    tgx_right = GetTgx(std::string("gfx/" + base + "r.tgx"));
    tgx_left = GetTgx(std::string("gfx/" + base + "l.tgx"));
  } else if (res == RESOLUTION_DYNAMIC) {
    // TODO
    tgx_right = GetTgx("gfx/edge1280r.tgx");
    tgx_left = GetTgx("gfx/edge1280l.tgx");
  }

  gm1_face = GetGm1("gm/face800-blank.gm1");
  gm1_scribe = GetGm1("gm/scribe.gm1");
  gm1_icons = GetGm1("gm/interface_buttons.gm1");
  gm1_floats = GetGm1("gm/floats.gm1");
}

void IngameGUI::RenderQuickMenu() {
  auto atlas = gm1_floats->GetTextureAtlas();
  int mouseX = GetMouseX(), mouseY = GetMouseY();

  SDL_Rect rect = atlas->Get(37);
  ui_compass.Transform(Rect<int>(int(mouseX - (rect.w / 2)),
                                 int(mouseY - rect.h - 25), rect.w, rect.h));

  rect = atlas->Get(124);
  ui_hide.Transform(Rect<int>(int(mouseX - rect.w - 45),
                              int(mouseY - (rect.h / 2)), rect.w, rect.h));

  rect = atlas->Get(45);
  ui_magnify.Transform(
      Rect<int>(int(mouseX + 45), int(mouseY - (rect.h / 2)), rect.w, rect.h));

  rect = atlas->Get(35);
  ui_lower.Transform(
      Rect<int>(int(mouseX - (rect.w / 2)), int(mouseY + 25), rect.w, rect.h));

  ui_compass.Render();
  ui_hide.Render();
  ui_magnify.Render();
  ui_lower.Render();
}

/**
 * Contains:
 * 1. Not selected index
 * 2. Mouse over index
 * 3. Selected index
 * 4. Relative x pos
 */
static uint16_t lut_ui_tabs[6][5] = {
    {7, 8, 9, 24},     {10, 11, 12, 60},  {13, 14, 15, 96},
    {16, 17, 18, 132}, {19, 20, 21, 168}, {22, 23, 24, 205},
};

void IngameGUI::RenderMenubar() {
  auto atlas = gm1_face->GetTextureAtlas();
  SDL_Rect rect = atlas->Get(0);
  int width, height = GetHeight(), menuOffsetX, menuOffsetY = height - rect.h;

  Resolution res = GetResolution();
  if (res == RESOLUTION_800x600) {
    menuOffsetX = 0;
    width = 800;
  } else {
    menuOffsetX = tgx_left->GetWidth();
    width = tgx_left->GetWidth() + 800 + tgx_right->GetWidth();
  }

  // Empty menu face //
  Rendering::Render(*atlas, menuOffsetX, menuOffsetY, &rect);

  // Scribe face //
  atlas = gm1_scribe->GetTextureAtlas();
  rect = atlas->Get(0);
  Rendering::Render(*atlas, menuOffsetX + 704, height - 200, &rect);

  // Left and right images (only 1240x768 and up) //
  if (res != RESOLUTION_800x600) {
    Rendering::Render(*tgx_right, tgx_left->GetWidth() + 800,
                      height - tgx_right->GetHeight());
    Rendering::Render(*tgx_left, 0, height - tgx_left->GetHeight());
  }

  // Calculate button positions //
  /*ui_disk.Translate  (menuOffsetX+800-287, menuOffsetY+16);
  ui_info.Translate  (menuOffsetX+800-287, menuOffsetY+54);
  ui_delete.Translate(menuOffsetX+800-287, menuOffsetY+86);
  ui_revert.Translate(menuOffsetX+800-287, menuOffsetY+112);

  // Render Menu buttons //
  atlas = gm1_icons->GetTextureAtlas();
  ui_disk.Render([&]() -> SDL_Rect {
      if (ui_disk.IsClicked()) {
      }
      if (ui_disk.IsMouseOver()) return atlas->Get(26);
      else return atlas->Get(25);
  });

  ui_info.Render([&]() -> SDL_Rect {
      if (ui_info.IsMouseOver()) return atlas->Get(28);
      else return atlas->Get(27);
  });

  ui_delete.Render([&]() -> SDL_Rect {
      if (ui_delete.IsMouseOver()) return atlas->Get(30);
      else return atlas->Get(29);
  });

  ui_revert.Render([&]() -> SDL_Rect {
      if (ui_revert.IsMouseOver()) return atlas->Get(69);
      else return atlas->Get(68);
  });

  // Translate and render the tab buttons //
  for (uint8_t i = 0; i < 6; i++) {
      ui_tabs[i].SetTexture(atlas.get());
      ui_tabs[i].Translate(menuOffsetX+lut_ui_tabs[i][3], menuOffsetY+109);
      ui_tabs[i].Scale(30, 35);
      ui_tabs[i].Render([&]() -> SDL_Rect {
          if (ui_tabs[i].IsClicked()) {
              currentTab = static_cast<MenuPage>(i);
          }
          else {
              if (ui_tabs[i].IsMouseOver())return atlas->Get(lut_ui_tabs[i][1]);
              else return atlas->Get(lut_ui_tabs[i][0]);
          }
          // Highlight the selected tab //
          if (currentTab == i) return atlas->Get(lut_ui_tabs[i][2]);
          return { 0, 0, 0, 0 };
      });
  }

  // Render the current pages content, TODO: may change when different building
  selected, etc. // switch (currentTab) { case MENU_CASTLE: { } break; case
  MENU_INDUSTRY: { } break; case MENU_FARM: { } break; case MENU_TOWN: { }
  break; case MENU_WEAPONS: { } break; case MENU_FOOD_PROCESSING: { } break;
  default:
      break;
  }*/
}

void IngameGUI::onEventReceive(Keyboard& keyEvent) {
  if (keyEvent.GetType() == KEYDOWN) {
    switch (keyEvent.Key().sym) {
      case SDLK_TAB:
        menubarShown = !menubarShown;
        break;
      case SDLK_ESCAPE:
        break;
      default:
        break;
    }
  }
}

void IngameGUI::onEventReceive(Mouse& mouseEvent) {
  if (mouseEvent.GetType() == BUTTONDOWN) {
    if (mouseEvent.RmbDown()) {
      rmbHolding = true;
    }

    if (mouseEvent.LmbDown()) {
      GrabMouse();
    }
  } else if (mouseEvent.GetType() == BUTTONUP) {
    if (mouseEvent.RmbUp()) {
      rmbHolding = false;
    }
  }
}
