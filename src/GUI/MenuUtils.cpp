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
static Texture _border_load;
static Table _table_load;

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

    _table_load.Create(2, 16);
    _table_load.SetRowName(0, L"Name");
    _table_load.SetRowName(1, L"Date");

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
    // todo
    return false;
}

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
        rect = atlas->Get(button_indices[style] + 1);
        Render(*atlas.get(), int(x)-5, int(y)-5, &rect);
        // TODO: blending doesn't work, this is only a white texture for _some_ reason
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

DialogResult GUI::QuitDialog()
{
    return IDLE;
}

DialogResult GUI::LoadDialog(std::string &name)
{
    int x = (1024 / 2) - (30*24 / 2);
    int y = (768  / 2) - (17*24 / 2);

    auto dim = GetStringPixelDim(L"Load", FONT_LARGE);

    Render(_border_load, x, y);
    RenderText(L"Load", x + 20 + (288 / 2) - (dim.first / 2), y+25, FONT_LARGE);

    RenderButton(BUTTON_3, L"Load", x+52, y+300);
    if(RenderButton(BUTTON_3, L"Back", x+52, y+340)) {
        return BACK;
    }

    _table_load.Render(x + 325, y + 50);

    return IDLE;
}
