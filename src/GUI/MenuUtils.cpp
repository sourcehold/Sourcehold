#include <GameManager.h>

#include <GUI/MenuUtils.h>
#include <GUI/StaticElement.h>
#include <GUI/Table.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>
#include <Rendering/Font.h>

#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>

#include <Events/Event.h>
#include <Events/Mouse.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;
using namespace GUI;

static std::shared_ptr<Gm1File> _gm_interface_icons3;
static std::shared_ptr<TgxFile> _tgx_border;
static SDL_Rect _border_rect;
static StrongholdEdition _ed;
static Resolution _res;
static Table _table_load, _table_combat;

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

    _table_load.Create(2, 16);
    _table_load.SetRowName(0, L"Name");
    _table_load.SetRowName(1, L"Date");

    _table_combat.Create(1, 21);
    _table_combat.Scrollable(false);
    _table_combat.RenderNames(false);

    return true;
}


/* Main menu functions */


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
    // todo
    return false;
}


/* Dialog functions */


// TODO: names?
enum DialogButton : uint8_t {
    BUTTON_1 = 0,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8
};

// !hack!
static class ButtonEventListener : protected EventConsumer<Mouse> {
public:
    bool clicked;
protected:
    void onEventReceive(Mouse &event) override {
        if(event.GetType() == MOUSE_BUTTONDOWN) {
            clicked = true;
        }else if(event.GetType() == MOUSE_BUTTONUP) {
            clicked = false;
        }
    }
} listener;

bool RenderButton(DialogButton style, const std::wstring& text, uint32_t x, uint32_t y)
{
    const static int button_indices[8] = {
        20, 23, 29, 32, 35, 38, 41, 44
    };

    auto atlas = _gm_interface_icons3->GetTextureAtlas();
    auto dim = GetStringPixelDim(text, FONT_SMALL);
    SDL_Rect rect = atlas->Get(button_indices[style]);;

    int mx = GetMouseX();
    int my = GetMouseY();

    // highlight
    int rw = GetTargetWidth() * rect.w;
    int rh = GetTargetHeight() * rect.h;
    int rx = ToCoordX(GetTargetX()) + x;
    int ry = ToCoordY(GetTargetY()) + y;

    bool clicked = false;
    if(mx > rx && mx < rx + rw && my > ry && my < ry + rh) {
        atlas->SetBlendMode(SDL_BLENDMODE_ADD);

        rect = atlas->Get(button_indices[style] + 1);
        Render(*atlas.get(), int(x)-5, int(y)-5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_MOD);

        rect = atlas->Get(button_indices[style] + 2);
        Render(*atlas.get(), int(x)-5, int(y)-5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_BLEND);

        clicked = listener.clicked;
    }

    rect = atlas->Get(button_indices[style]);

    // button
    Render(*atlas.get(), int(x), int(y), &rect);

    // text
    RenderText(text, int(x + (rect.w / 2) - (dim.first / 2)), int(y + (rect.h / 2) - (dim.second / 2))+2, FONT_SMALL);

    return clicked;
}

void RenderDialogBorder(int x, int y, int nx, int ny)
{
    auto atlas = _gm_interface_icons3->GetTextureAtlas();
    SDL_Rect rect;

    /* Render alpha masks */

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    // corners
    rect = atlas->Get(3);
    Render(*atlas, x, y, &rect);
    rect = atlas->Get(15);
    Render(*atlas, x, y+ny*24, &rect);
    rect = atlas->Get(5);
    Render(*atlas, x+nx*24, y, &rect);
    rect = atlas->Get(17);
    Render(*atlas, x+nx*24, y+ny*24, &rect);

    // edges
    for(int ix = x+24; ix < x+nx*24; ix+=24) {
        rect = atlas->Get(4);
        Render(*atlas, ix, y, &rect);
        rect = atlas->Get(16);
        Render(*atlas, ix, y+ny*24, &rect);
    }
    for(int iy = y+24; iy < y+ny*24; iy+=24) {
        rect = atlas->Get(9);
        Render(*atlas, x, iy, &rect);
        rect = atlas->Get(11);
        Render(*atlas, x+nx*24, iy, &rect);
    }

    /* Render color */

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    // corners
    rect = atlas->Get(0);
    Render(*atlas, x, y, &rect);
    rect = atlas->Get(12);
    Render(*atlas, x, y+ny*24, &rect);
    rect = atlas->Get(2);
    Render(*atlas, x+nx*24, y, &rect);
    rect = atlas->Get(14);
    Render(*atlas, x+nx*24, y+ny*24, &rect);

    // edges
    for(int ix = x+24; ix < x+nx*24; ix+=24) {
        rect = atlas->Get(1);
        Render(*atlas, ix, y, &rect);
        rect = atlas->Get(13);
        Render(*atlas, ix, y+ny*24, &rect);
    }
    for(int iy = y+24; iy < y+ny*24; iy+=24) {
        rect = atlas->Get(6);
        Render(*atlas, x, iy, &rect);
        rect = atlas->Get(8);
        Render(*atlas, x+nx*24, iy, &rect);
    }

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);

    RenderRect(Rect<int>{ x+8, y+8, 8+nx*24, 8+ny*24 }, 0, 0, 0, 128, true);
}

enum class Deco {
    LARGE,
    SMALL
};

void RenderDeco(Deco type, int x, int y)
{
    auto atlas = _gm_interface_icons3->GetTextureAtlas();

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    SDL_Rect rect = atlas->Get(91);
    Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void RenderDialogTextBox(int x, int y, int w, int h, const std::wstring &text, bool deco = false)
{
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 24, 80, 24, 152, true);
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 247, 235, 198, 255, false);

    auto dim = GetStringPixelDim(text, FONT_LARGE);
    RenderText(text, x + 20 + (w / 2) - (dim.first / 2), y+25, FONT_LARGE);

    if(deco) {
        RenderDeco(Deco::LARGE,      x + 11 , y + (h/2)-11);
        RenderDeco(Deco::LARGE, x + (w - 16), y + (h/2)-11);
    }
}

DialogResult GUI::QuitDialog()
{
    int x = (1024 / 2) - (18*24 / 2);
    int y = (768  / 2) - (7*24  / 2);

    RenderDialogBorder(x, y, 18, 6);
    RenderDialogTextBox(x, y, 433, 64, L"Exit Stronghold", true);

    if(RenderButton(BUTTON_4, L"Yes", x+105, y+105)) {
        return QUIT;
    }

    if(RenderButton(BUTTON_4, L"No", x+245, y+105)) {
        return BACK;
    }

    return IDLE;
}

DialogResult GUI::LoadDialog(std::string &name)
{
    int x = (1024 / 2) - (30*24 / 2);
    int y = (768  / 2) - (17*24 / 2);

    RenderDialogBorder(x, y, 30, 17);
    RenderDialogTextBox(x, y, 288, 64, L"Load", true);

    if(RenderButton(BUTTON_3, L"Load", x+52, y+300)) {
        // todo
    }
    if(RenderButton(BUTTON_3, L"Back", x+52, y+340)) {
        return BACK;
    }

    _table_load.Render(x + 330, y + 28, 363);

    return IDLE;
}

DialogResult GUI::CombatMenuDialog()
{
    int x = (1024 / 2) - (416 / 2);
    int y = 90;

    RenderDialogTextBox(x, y, 416, 64, L"Mission", true);

    _table_combat.Render(x + 37, y + 90, 363);

    return IDLE;
}

