#include <codecvt>

#include "GameManager.h"

#include "GUI/MenuUtils.h"
#include "GUI/Widgets/StaticElement.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/LineEdit.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Font.h"

#include "Parsers/Gm1File.h"
#include "Parsers/TgxFile.h"
#include "Parsers/CfgFile.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;
using namespace GUI;

std::shared_ptr<TgxFile> tgx_border;

void GUI::InitMenuUtils() {
  if (GetEdition() == STRONGHOLD_HD) {
    tgx_border = GetTgx("gfx/SH1_Back.tgx");
  }
}

void GUI::RenderMenuText(const std::wstring &text) {
  if (text.empty())
    return;

  auto interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();

  auto rect = interface_icons->Get(18);
  Renderer::Instance().Render(*interface_icons, Vector2<int>{312, 400}, &rect);
  RenderText(text.substr(0, 1), 317, 406, FONT_SMALL, true);
  RenderText(text.substr(1, text.size()), 338, 406, FONT_SMALL);
}

void GUI::RenderMenuBorder() {
  if (GetEdition() == STRONGHOLD_HD) {
    SDL_Rect border_rect;

#if 0
        /**
         * Render the border 'zoomed in' so that the
         * menu can be placed in the middle without scaling.
         */
        border_rect.x = (1920 - GetWidth()) / 2;
        border_rect.y = (1200 - GetHeight()) / 2;
        border_rect.w = GetWidth();
        border_rect.h = GetHeight();
#endif

    Renderer::Instance().Render(
        *tgx_border, Vector2<int>{(GetWidth() - 1920) / 2,
                                  (GetHeight() - 1200) / 2} /*, &border_rect*/);
  }
}

bool GUI::CheckButtonCollision(uint32_t rx, uint32_t ry) {
  // todo
  return false;
}

std::shared_ptr<Dialog> GUI::QuitDialog() {
  auto res = std::make_shared<Dialog>(WidgetLayout::HORIZONTAL, 18, 6,
                                      GetString(T_MAIN_MENU, 5), true,
                                      Deco::LARGE, 18);
  // Yes //
  res->Add<Button>(BUTTON_4, GetString(T_GAME_OPTIONS, 22))
      ->SetOnClick([](Mouse &m) {
        if (m.LmbDown()) {
          SaveConfig();
          ExitGame();
        }
      });

  // No //
  res->Add<Button>(BUTTON_4, GetString(T_GAME_OPTIONS, 23))
      ->SetOnClick([res](Mouse &m) {
        if (m.LmbDown() && res->onExit) {
          res->onExit();
        }
      });

  return res;
}

std::shared_ptr<Dialog> GUI::LoadDialog() {
  auto res = std::make_shared<Dialog>(WidgetLayout::HORIZONTAL, 30, 17,
                                      GetString(T_GAME_OPTIONS, 2), true,
                                      Deco::LARGE, 12);
  // TODO
  auto table = res->Add<Table>(16, 2);
  table->SetColName(0, GetString(T_GAME_OPTIONS, 27));
  table->SetColName(1, GetString(T_GAME_OPTIONS, 28));

  return res;
}

std::shared_ptr<Dialog> GUI::CombatMenuDialog() {
  auto res = std::make_shared<Dialog>(WidgetLayout::VERTICAL, 17, 22,
                                      GetString(T_MAIN_MENU, 15), true,
                                      Deco::LARGE, 17, false);

  auto table = res->Add<Table>(21, 1);
  for (int i = 0; i < 21; i++) {
    table->SetText(
        i, 0,
        std::to_wstring(i + 1) + L" " + GetString(T_MISSION_NAMES, 1 + i));
  }

  table->RenderNames(false);

  return res;
}
