#include "GUI/Widgets/Button.h"
#include "GUI/MenuUtils.h"

#include "Rendering/Font.h"
#include "Rendering/Display.h"

#include "GameManager.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::Game;

Button::Button(DialogButton style, const std::wstring& text) :
    Widget(),
    style_{style},
    text_{text},
    selected_{false}
{
}

Button::~Button()
{
}

void Button::Update(Rect<int> constraints)
{
    const static uint32_t button_indices[] = {
        20, 23, 29, 32, 35, 38, 41, 44, 96,
    };

    [[maybe_unused]] auto dim = GetStringPixelDim(text_, FONT_SMALL);
    auto interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();

    auto atlas = interface_icons;
    SDL_Rect rect = atlas->Get(button_indices[style_]);

    int mx = GetMouseX();
    int my = GetMouseY();

    int x = constraints.x + ((constraints.w - rect.w) / 2);
    int y = constraints.y + ((constraints.h - rect.h) / 2);

    // highlight
    auto target = GetTarget();
    int rw = rect.w;
    int rh = rect.h;
    int rx = target.x + x;
    int ry = target.y + y;

    selected_ = false;
    if (mx > rx && mx < rx + rw && my > ry && my < ry + rh) {
        atlas->SetBlendMode(SDL_BLENDMODE_ADD);

        rect = atlas->Get(button_indices[style_] + 1);
        Rendering::Render(*atlas.get(), x - 5, y - 5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_MOD);

        rect = atlas->Get(button_indices[style_] + 2);
        Rendering::Render(*atlas.get(), x - 5, y - 5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
        selected_ = true;
    }

    rect = atlas->Get(button_indices[style_]);

    // button
    atlas->SetAlphaMod(140);
    Rendering::Render(*atlas.get(), x, y, &rect);
    atlas->SetAlphaMod(255);

    // text
    Rendering::RenderText(text_, Rect<uint32_t>(static_cast<uint32_t>(x) + 4, static_cast<uint32_t>(y) + 4, static_cast<uint32_t>(rect.w) - 4, static_cast<uint32_t>(rect.h) - 4), Align::CENTER, FONT_LARGE, false);
}

void Button::onEventReceive(Mouse& mouse_event)
{
    if (mouse_event.type == BUTTONDOWN && handler_ && selected_) {
        handler_(mouse_event);
        selected_ = false;
    }
}
