#include "GUI/Dialog.h"
#include "Rendering/Font.h"
#include "GameManager.h"

using namespace Sourcehold;
using namespace GUI;
using namespace Game;
using namespace Rendering;

Dialog::~Dialog() {}
Dialog::Dialog(WidgetLayout l, int nx, int ny) :
    Container(l),
    nx(nx), ny(ny)
{}

DialogResult Dialog::Update(Dialog::Position pos, int offX, int offY)
{
    int w = nx * MENU_TILE_DIM;
    int h = ny * MENU_TILE_DIM;

    int x = (pos == Dialog::CENTRE) ? ((ToCoordX( GetTargetWidth()) - w) / 2) + offX : offX;
    int y = (pos == Dialog::CENTRE) ? ((ToCoordY(GetTargetHeight()) - h) / 2) + offY : offY;

    icons = GetGm1("gm/interface_icons3.gm1");

    RenderBorder(x, y, nx, ny);
    RenderTextBox(x, y, w, 64, GetString(T_MAIN_MENU, 5), true);

    // Render the member widgets in the available area of the dialog //
    w += MENU_TILE_DIM; // TODO
    h += MENU_TILE_DIM;
    Container::Update(Rect<int>(x, y+64, w, h-64));

    return DialogResult::IDLE;
}

void Dialog::RenderBorder(int x, int y, int nx, int ny)
{
    SDL_Rect rect;
    auto atlas = icons->GetTextureAtlas();

    // background
    RenderRect(Rect<int>{ x + 8, y + 8, 8 + nx * MENU_TILE_DIM, 8 + ny * MENU_TILE_DIM }, 0, 0, 0, 128, true);

    /* Render alpha masks */
    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    // corners
    rect = atlas->Get(3);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(15);
    Rendering::Render(*atlas, x, y + ny * MENU_TILE_DIM, &rect);
    rect = atlas->Get(5);
    Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, y, &rect);
    rect = atlas->Get(17);
    Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, y + ny * MENU_TILE_DIM, &rect);

    // edges
    for (int ix = x + MENU_TILE_DIM; ix < x + nx * MENU_TILE_DIM; ix += MENU_TILE_DIM) {
        rect = atlas->Get(4);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(16);
        Rendering::Render(*atlas, ix, y + ny * MENU_TILE_DIM, &rect);
    }
    for (int iy = y + MENU_TILE_DIM; iy < y + ny * MENU_TILE_DIM; iy += MENU_TILE_DIM) {
        rect = atlas->Get(9);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(11);
        Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, iy, &rect);
    }

    /* Render color */
    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    // corners
    rect = atlas->Get(0);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(12);
    Rendering::Render(*atlas, x, y + ny * MENU_TILE_DIM, &rect);
    rect = atlas->Get(2);
    Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, y, &rect);
    rect = atlas->Get(14);
    Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, y + ny * MENU_TILE_DIM, &rect);

    // edges
    for (int ix = x + MENU_TILE_DIM; ix < x + nx * MENU_TILE_DIM; ix += MENU_TILE_DIM) {
        rect = atlas->Get(1);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(13);
        Rendering::Render(*atlas, ix, y + ny * MENU_TILE_DIM, &rect);
    }
    for (int iy = y + MENU_TILE_DIM; iy < y + ny * MENU_TILE_DIM; iy += MENU_TILE_DIM) {
        rect = atlas->Get(6);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(8);
        Rendering::Render(*atlas, x + nx * MENU_TILE_DIM, iy, &rect);
    }

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderDeco(Deco type, int x, int y)
{
    auto atlas = icons->GetTextureAtlas();

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    SDL_Rect rect = atlas->Get(91);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderTextBox(int x, int y, int w, int h, const std::wstring& text, bool deco)
{
    RenderRect(Rect<int>{ x + 8, y + 8, w + 8, h + 8 },  24,  80,  24, 200, true);
    RenderRect(Rect<int>{ x + 8, y + 8, w + 8, h + 8 }, 247, 235, 198, 255, false);

    auto dim = GetStringPixelDim(text, FONT_LARGE);
    RenderText(text, x + 20 + (w / 2) - (dim.first / 2), y + 25, FONT_LARGE);

    if (deco) {
        RenderDeco(Deco::LARGE, x + 11, y + (h / 2) - 11);
        RenderDeco(Deco::LARGE, x + (w - 16), y + (h / 2) - 11);
    }
}
