#include <codecvt>

#include "GameManager.h"

#include "GUI/MenuUtils.h"
#include "GUI/Widgets/StaticElement.h"
#include "GUI/Widgets/Button.h"

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

void GUI::InitMenuUtils()
{    
    if (GetEdition() == STRONGHOLD_HD) {
        tgx_border = GetTgx("gfx/SH1_Back.tgx");
    }
}

void GUI::RenderMenuText(const std::wstring &text)
{
    if(text.empty()) return;

    auto interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();

    auto rect = interface_icons->Get(18);
    Render(*interface_icons, 312, 400, &rect);
    RenderText(text.substr(0,1), 317, 406, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 338, 406, FONT_SMALL);
}

void GUI::RenderMenuBorder()
{
    if(GetEdition() == STRONGHOLD_HD) {
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

        Render(*tgx_border, (GetWidth() - 1920) / 2, (GetHeight() - 1200) / 2/*, &border_rect*/);
    }
}

bool GUI::CheckButtonCollision(uint32_t rx, uint32_t ry)
{
    // todo
    return false;
}

std::shared_ptr<Dialog> GUI::QuitDialog()
{
    std::shared_ptr<Dialog> res = std::make_shared<Dialog>(
        WidgetLayout::HORIZONTAL, 18, 6
    );
    res->Add(std::make_shared<Button>(BUTTON_4, GetString(T_GAME_OPTIONS, 22))); // Yes
    res->Add(std::make_shared<Button>(BUTTON_4, GetString(T_GAME_OPTIONS, 23))); // No
    return res;
}
