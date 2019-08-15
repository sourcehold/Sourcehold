#include <GUI/LineEdit.h>
#include <Rendering/Font.h>
#include <GameManager.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;

LineEdit::LineEdit(const std::wstring& line) :
    EventConsumer<Keyboard>()
{
    this->line = line;
}

LineEdit::~LineEdit()
{
}

void LineEdit::Init()
{
    atlas = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();
    parts[0] = atlas->Get(114);
    parts[1] = atlas->Get(115);
    parts[2] = atlas->Get(116);

    cp = line.size();
}

void LineEdit::BeginInput()
{
    SDL_StartTextInput();
}

void LineEdit::EndInput()
{
    SDL_StopTextInput();
}

void LineEdit::Render(int x, int y, int nx)
{
    if(nx <= 2) return;

    auto dim = GetStringPixelDim(line.substr(0, cp), FONT_SMALL);

    atlas->SetAlphaMod(127);

    ::Render(*atlas, x, y, &parts[0]);
    ::Render(*atlas, x+10*nx, y, &parts[2]);

    for(int px = 10; px < nx*10; px += 10) {
        ::Render(*atlas, x+px, y, &parts[1]);
    }

    atlas->SetAlphaMod(255);

    RenderText(line, x + 4, y + 6, FONT_SMALL);
    RenderRect(Rect<int>(x+dim.first, y, 2, 34), 24, 80, 24, 255, true);
}

void LineEdit::onEventReceive(Keyboard &event)
{
    if(event.type == KEYBOARD_KEYDOWN) {
        if(event.Key().sym == SDLK_BACKSPACE) {
            if(line.size() >= 1 && cp) {
                line.erase(line.begin() + cp - 1);
                cp--;
            }
        }else if(event.Key().sym == SDLK_DELETE) {
            if(cp < line.size()) {
                line.erase(line.begin() + cp);
            }
        }else if(event.Key().sym == SDLK_LEFT) {
            if(cp) cp--;
        }else if(event.Key().sym == SDLK_RIGHT) {
            if(cp < line.size()) cp++;
        }
    }else if(event.type == KEYBOARD_TEXTINPUT && cp <= line.size()) {
        char c = event.Get().text.text[0];
        line.insert(line.begin() + cp, c);
        cp++;
    }
}
