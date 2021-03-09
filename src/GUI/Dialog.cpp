#include "GUI/Dialog.h"
#include "Rendering/Font.h"
#include "GameManager.h"

using namespace Sourcehold;
using namespace GUI;
using namespace Game;
using namespace Rendering;

Dialog::~Dialog() {}
Dialog::Dialog(WidgetLayout l, int nx, int ny, const std::wstring& text, bool textbox, Deco deco, int textboxW, bool border) :
    Container(l),
    nx_(nx), ny_(ny),
    tw_{textboxW},
    text_box_{textbox},
    border_{border},
    deco_{deco},
    text_{text}
{}

void Dialog::Render(Dialog::Position pos, int offX, int offY)
{
    int w = nx_ * menu_title_dim_;
    int h = ny_ * menu_title_dim_;

    int x = (pos == Dialog::CENTRE) ? ((GetWidth()  - w) / 2) + offX : offX;
    int y = (pos == Dialog::CENTRE) ? ((GetHeight() - h) / 2) + offY : offY;

    icons_ = GetGm1("gm/interface_icons3.gm1");

    if(border_)  RenderBorder(x, y, nx_, ny_);
    if(text_box_) RenderTextBox(x, y, tw_*24, 64, text_, deco_);

    // Render the member widgets in the available area of the dialog //
    w += menu_title_dim_; // TODO
    h += menu_title_dim_;
    Container::Update(Rect<int>(x+margin_x_, y+margin_y_+(text_box_?64:0), w-margin_x_*2, h-margin_y_*2-(text_box_?64:0)));
}

void Dialog::RenderBorder(int x, int y, int nx, int ny)
{
    SDL_Rect rect;
    auto atlas = icons_->GetTextureAtlas();

    // background
    RenderRect(Rect<int>{ x + 8, y + 8, 8 + nx * menu_title_dim_, 8 + ny * menu_title_dim_ }, 0, 0, 0, 128, true);

    /* Render alpha masks */
    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    // corners
    rect = atlas->Get(3);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(15);
    Rendering::Render(*atlas, x, y + ny * menu_title_dim_, &rect);
    rect = atlas->Get(5);
    Rendering::Render(*atlas, x + nx * menu_title_dim_, y, &rect);
    rect = atlas->Get(17);
    Rendering::Render(*atlas, x + nx * menu_title_dim_, y + ny * menu_title_dim_, &rect);

    // edges
    for (int ix = x + menu_title_dim_; ix < x + nx * menu_title_dim_; ix += menu_title_dim_) {
        rect = atlas->Get(4);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(16);
        Rendering::Render(*atlas, ix, y + ny * menu_title_dim_, &rect);
    }
    for (int iy = y + menu_title_dim_; iy < y + ny * menu_title_dim_; iy += menu_title_dim_) {
        rect = atlas->Get(9);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(11);
        Rendering::Render(*atlas, x + nx * menu_title_dim_, iy, &rect);
    }

    /* Render color */
    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    // corners
    rect = atlas->Get(0);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(12);
    Rendering::Render(*atlas, x, y + ny * menu_title_dim_, &rect);
    rect = atlas->Get(2);
    Rendering::Render(*atlas, x + nx * menu_title_dim_, y, &rect);
    rect = atlas->Get(14);
    Rendering::Render(*atlas, x + nx * menu_title_dim_, y + ny * menu_title_dim_, &rect);

    // edges
    for (int ix = x + menu_title_dim_; ix < x + nx * menu_title_dim_; ix += menu_title_dim_) {
        rect = atlas->Get(1);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(13);
        Rendering::Render(*atlas, ix, y + ny * menu_title_dim_, &rect);
    }
    for (int iy = y + menu_title_dim_; iy < y + ny * menu_title_dim_; iy += menu_title_dim_) {
        rect = atlas->Get(6);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(8);
        Rendering::Render(*atlas, x + nx * menu_title_dim_, iy, &rect);
    }

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderDeco(Deco type, int x, int y)
{
    if (type == Deco::NONE) return;

    auto atlas = icons_->GetTextureAtlas();

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    SDL_Rect rect = atlas->Get(91);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderTextBox(int x, int y, int w, int h, const std::wstring& text, Deco deco)
{
    RenderRect(Rect<int>{ x + 8, y + 8, w + 8, h + 8 },  24,  80,  24, 200, true);
    RenderRect(Rect<int>{ x + 8, y + 8, w + 8, h + 8 }, 247, 235, 198, 255, false);

    auto dim = GetStringPixelDim(text, FONT_LARGE);
    RenderText(text, static_cast<uint32_t>(x + 20 + (w / 2)) - (dim.first / 2), static_cast<uint32_t>(y + 25), FONT_LARGE);

    RenderDeco(deco, x + 11, y + (h / 2) - 11);
    RenderDeco(deco, x + (w - 16), y + (h / 2) - 11);
}
