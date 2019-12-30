#include "GUI/Widgets/Button.h"

#include "Rendering/Font.h"
#include "Rendering/Display.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Button::Button(DialogButton style, uint32_t x, uint32_t y, const std::wstring& text) :
    style{style},
    x{x}, y{y},
    text{text}
{
}

Button::~Button()
{
}

bool Button::Update()
{
    const static int button_indices[] = {
        20, 23, 29, 32, 35, 38, 41, 44, 96,
    };

    auto dim = GetStringPixelDim(text, FONT_SMALL);
    auto interface_icons = GetInterfaceIcons();

    auto atlas = interface_icons;
    SDL_Rect rect = atlas->Get(button_indices[style]);

    int mx = GetMouseX();
    int my = GetMouseY();

    // highlight
    int rw = GetTargetWidth() * rect.w;
    int rh = GetTargetHeight() * rect.h;
    int rx = ToCoordX(GetTargetX()) + x;
    int ry = ToCoordY(GetTargetY()) + y;

    bool selected = false;
    if (mx > rx&& mx < rx + rw && my > ry&& my < ry + rh) {
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

    // TODO
    if (clicked && selected) {
        clicked = false;
        return true;
    }
    else return false;
}

void Button::onEventReceive(Mouse& event)
{
    clicked = event.LmbDown() && !event.RmbDown();
}
