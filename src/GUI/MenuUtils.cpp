#include <GameManager.h>

#include <GUI/MenuUtils.h>
#include <GUI/StaticElement.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>
#include <Rendering/Font.h>

#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

static std::shared_ptr<Gm1File> _gm_interface_icons3;
static std::shared_ptr<TgxFile> _tgx_border;
static SDL_Rect _border_rect;
static StrongholdEdition _ed;
static Resolution _res;
static Texture _border_load;

bool GUI::InitializeUtils()
{
    _gm_interface_icons3 = GetGm1(GetDirectory() / "gm/interface_icons3.gm1");
    _ed = GetEdition();
    _res = GetResolution();
    if(_ed == STRONGHOLD_HD) {
        _tgx_border = GetTgx(GetDirectory() / "gfx/SH1_Back.tgx");
        /**
         * Render the border 'zoomed in' so that theP
         * menu can be placed in the middle without scaling.
         */
        _border_rect.x = (1920 - GetWidth()) / 2;
        _border_rect.y = (1200 - GetHeight()) / 2;
        _border_rect.w = GetWidth();
        _border_rect.h = GetHeight();
    }

    auto atlas = _gm_interface_icons3->GetTextureAtlas();

    /* Precalculate blending */
    Texture deco;
    deco.AllocNewTarget(27, 44);

    SetTarget(&deco, Rect<double>{ 0.0, 0.0, NormalizeX(27), NormalizeY(44) });

    SDL_Rect rect = atlas->Get(91);
    Render(*atlas, &rect);

    //SDL_BlendMode mode = GetAlphaKeyBlendMode();
    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Render(*atlas, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);

    _border_load.AllocNewTarget(30*24, 17*24);

    SetTarget(&_border_load, Rect<double>{ 0.0, 0.0, NormalizeX(30*24), NormalizeY(17*24) });

    RenderRect(Rect<int>{ 0, 0, 30*24, 17*24 }, 0, 0, 0, 127, true);

    /* Corners */
    rect = atlas->Get(0);
    Render(*atlas, 0, 0, &rect);
    rect = atlas->Get(12);
    Render(*atlas, 0, 16*24, &rect);
    rect = atlas->Get(2);
    Render(*atlas, 29*24, 0, &rect);
    rect = atlas->Get(14);
    Render(*atlas, 29*24, 16*24, &rect);

    /* Edges */
    for(int ix = 24; ix < 29*24; ix+=24) {
        rect = atlas->Get(1);
        Render(*atlas, ix, 0, &rect);
        rect = atlas->Get(13);
        Render(*atlas, ix, 16*24, &rect);
    }
    for(int iy = 24; iy < 16*24; iy+=24) {
        rect = atlas->Get(6);
        Render(*atlas, 0, iy, &rect);
        rect = atlas->Get(8);
        Render(*atlas, 29*24, iy, &rect);
    }

    /* Title box */
    RenderRect(Rect<int>{ 8, 8, 8+288, 8+64 }, 24, 80, 24, 152, true);
    RenderRect(Rect<int>{ 8, 8, 8+288, 8+64 }, 247, 235, 198, 255, false);

    /* Decoration, TODO */
    Render(deco, 11, 17);
    Render(deco, 266, 17);

    ResetTarget();

    return true;
}

void GUI::RenderMenuText(const std::wstring &text)
{
    if(text.empty()) return;

    std::shared_ptr<TextureAtlas> interface_icons = _gm_interface_icons3->GetTextureAtlas();
    auto rect = interface_icons->Get(18);
    Render(*interface_icons, 312, 400, &rect);
    RenderText(text.substr(0,1), 317, 406, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 338, 406, FONT_SMALL);
}

void GUI::RenderMenuBorder()
{
    Render(*_tgx_border, &_border_rect);
}

bool GUI::CheckButtonCollision(uint32_t rx, uint32_t ry)
{
    return false;
}

void GUI::RenderMenuBox(MenuBox style, const std::wstring &title)
{
    int x, y;

    switch(style) {
    case MENUBOX_LOAD_GAME: {
        x = ToCoordX(0.0);
        y = ToCoordY(0.0);

        Render(_border_load, x, y);
    }
    break;
    case MENUBOX_BUILDER: {

    } break;
    case MENUBOX_CAMPAIGN: {
    } break;
    case MENUBOX_ENDGAME: {

    } break;
    case MENUBOX_ESC: {

    } break;
    default:
        break;
    }
}
