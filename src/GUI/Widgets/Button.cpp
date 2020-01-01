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
    style{style},
    text{text},
    selected{false}
{
}

Button::~Button()
{
}

void Button::Update(Rect<int> constraints)
{
    const static int button_indices[] = {
        20, 23, 29, 32, 35, 38, 41, 44, 96,
    };

    auto dim = GetStringPixelDim(text, FONT_SMALL);
    auto interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();

    auto atlas = interface_icons;
    SDL_Rect rect = atlas->Get(button_indices[style]);

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

    selected = false;
    if (mx > rx && mx < rx + rw && my > ry && my < ry + rh) {
        atlas->SetBlendMode(SDL_BLENDMODE_ADD);

        rect = atlas->Get(button_indices[style] + 1);
        Rendering::Render(*atlas.get(), int(x) - 5, int(y) - 5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_MOD);

        rect = atlas->Get(button_indices[style] + 2);
        Rendering::Render(*atlas.get(), int(x) - 5, int(y) - 5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
        selected = true;
    }

    rect = atlas->Get(button_indices[style]);

    // button
    atlas->SetAlphaMod(140);
    Rendering::Render(*atlas.get(), int(x), int(y), &rect);
    atlas->SetAlphaMod(255);

    // text
    Rendering::RenderText(text, Rect<int>(x + 4, y + 4, rect.w - 4, rect.h - 4), Align::CENTER, FONT_LARGE, false);
}

void Button::onEventReceive(Mouse& event)
{
    if (event.type == BUTTONDOWN && handler && selected) {
        handler(event);
    }
}
