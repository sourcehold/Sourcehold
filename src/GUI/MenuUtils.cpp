#include <codecvt>

#include "GameManager.h"

#include "GUI/MenuUtils.h"
#include "GUI/Widgets/StaticElement.h"

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

std::shared_ptr<TextureAtlas> interface_icons;
std::shared_ptr<TgxFile> tgx_border;

void GUI::InitMenuUtils()
{
    interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();
    
    if (GetEdition() == STRONGHOLD_HD) {
        tgx_border = GetTgx("gfx/SH1_Back.tgx");
    }
}

std::shared_ptr<TextureAtlas> GUI::GetInterfaceIcons()
{
    return interface_icons;
}

void GUI::RenderMenuText(const std::wstring &text)
{
    if(text.empty()) return;

    auto rect = interface_icons->Get(18);
    Render(*interface_icons, 312, 400, &rect);
    RenderText(text.substr(0,1), 317, 406, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 338, 406, FONT_SMALL);
}

void GUI::RenderMenuBorder()
{
    if(GetEdition() == STRONGHOLD_HD) {
        SDL_Rect border_rect;

        /**
         * Render the border 'zoomed in' so that the
         * menu can be placed in the middle without scaling.
         */
        border_rect.x = (1920 - GetWidth()) / 2;
        border_rect.y = (1200 - GetHeight()) / 2;
        border_rect.w = GetWidth();
        border_rect.h = GetHeight();

        Render(*tgx_border, &border_rect);
    }
}

bool GUI::CheckButtonCollision(uint32_t rx, uint32_t ry)
{
    // todo
    return false;
}

/* Dialog functions */

static const int MENU_TILE_DIM = 24; // Size of one 'tile' menus are made of
void GUI::RenderDialogBorder(int x, int y, int nx, int ny)
{
    auto atlas = interface_icons;
    SDL_Rect rect;

    // background
    RenderRect(Rect<int>{ x+8, y+8, 8+nx*MENU_TILE_DIM, 8+ny*MENU_TILE_DIM }, 0, 0, 0, 128, true);

    /* Render alpha masks */
    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    // corners
    rect = atlas->Get(3);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(15);
    Rendering::Render(*atlas, x, y+ny*MENU_TILE_DIM, &rect);
    rect = atlas->Get(5);
    Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, y, &rect);
    rect = atlas->Get(17);
    Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, y+ny*MENU_TILE_DIM, &rect);

    // edges
    for(int ix = x+MENU_TILE_DIM; ix < x+nx*MENU_TILE_DIM; ix+=MENU_TILE_DIM) {
        rect = atlas->Get(4);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(16);
        Rendering::Render(*atlas, ix, y+ny*MENU_TILE_DIM, &rect);
    }
    for(int iy = y+MENU_TILE_DIM; iy < y+ny*MENU_TILE_DIM; iy+=MENU_TILE_DIM) {
        rect = atlas->Get(9);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(11);
        Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, iy, &rect);
    }

    /* Render color */
    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    // corners
    rect = atlas->Get(0);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(12);
    Rendering::Render(*atlas, x, y+ny*MENU_TILE_DIM, &rect);
    rect = atlas->Get(2);
    Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, y, &rect);
    rect = atlas->Get(14);
    Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, y+ny*MENU_TILE_DIM, &rect);

    // edges
    for(int ix = x+MENU_TILE_DIM; ix < x+nx*MENU_TILE_DIM; ix+=MENU_TILE_DIM) {
        rect = atlas->Get(1);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(13);
        Rendering::Render(*atlas, ix, y+ny*MENU_TILE_DIM, &rect);
    }
    for(int iy = y+MENU_TILE_DIM; iy < y+ny*MENU_TILE_DIM; iy+=MENU_TILE_DIM) {
        rect = atlas->Get(6);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(8);
        Rendering::Render(*atlas, x+nx*MENU_TILE_DIM, iy, &rect);
    }

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void GUI::RenderDeco(Deco type, int x, int y)
{
    auto atlas = interface_icons;

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    SDL_Rect rect = atlas->Get(91);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void GUI::RenderDialogTextBox(int x, int y, int w, int h, const std::wstring &text, bool deco)
{
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 24, 80, 24, 200, true);
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 247, 235, 198, 255, false);

    auto dim = GetStringPixelDim(text, FONT_LARGE);
    RenderText(text, x + 20 + (w / 2) - (dim.first / 2), y+25, FONT_LARGE);

    if(deco) {
        RenderDeco(Deco::LARGE,      x + 11 , y + (h/2)-11);
        RenderDeco(Deco::LARGE, x + (w - 16), y + (h/2)-11);
    }
}
