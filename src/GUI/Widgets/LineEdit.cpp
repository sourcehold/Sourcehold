#include "GUI/Widgets/LineEdit.h"
#include "Rendering/Font.h"
#include "GameManager.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;

LineEdit::LineEdit(int nx, const std::wstring& line) :
    EventConsumer<Keyboard>(),
    line_{line},
    nx_{nx},
    cp_{0}
{}

LineEdit::~LineEdit()
{
}

void LineEdit::Init()
{
    cp_ = static_cast<int>(line_.size());
}

void LineEdit::BeginInput()
{
    SDL_StartTextInput();
}

void LineEdit::EndInput()
{
    SDL_StopTextInput();
}

void LineEdit::Update(Rect<int> constraints)
{
    std::shared_ptr<TextureAtlas> atlas = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();
    SDL_Rect parts[3];
    parts[0] = atlas->Get(114);
    parts[1] = atlas->Get(115);
    parts[2] = atlas->Get(116);

    auto dim = GetStringPixelDim(line_.substr(0, static_cast<size_t>(cp_)), FONT_SMALL);

    int x = constraints.x + ((constraints.w - LINE_SEGMENT_W*nx_) / 2);
    int y = constraints.y + ((constraints.h - parts[0].h) / 2);

    atlas->SetAlphaMod(127);

    ::Render(*atlas, x, y, &parts[0]);
    ::Render(*atlas, x+ LINE_SEGMENT_W * nx_, y, &parts[2]);

    for(int px = LINE_SEGMENT_W; px < nx_ * LINE_SEGMENT_W; px += LINE_SEGMENT_W) {
        ::Render(*atlas, x+px, y, &parts[1]);
    }

    atlas->SetAlphaMod(255);

    // Text contents
    RenderText(line_, static_cast<uint32_t>(x) + 4, static_cast<uint32_t>(y) + 6, FONT_SMALL);

    // Cursor
    RenderRect(Rect<int>(x+static_cast<int>(dim.first), y, 2, 34), 24, 80, 24, 255, true);
}

void LineEdit::onEventReceive(Keyboard &event)
{
    if(event.type == KEYDOWN) {
        if(event.Key().sym == SDLK_BACKSPACE) {
            if(line_.size() >= 1 && cp_) {
                line_.erase(line_.begin() + cp_ - 1);
                cp_--;
            }
        }else if(event.Key().sym == SDLK_DELETE) {
            if(cp_ < static_cast<int>(line_.size())) {
                line_.erase(line_.begin() + cp_);
            }
        }else if(event.Key().sym == SDLK_LEFT) {
            if(cp_) cp_--;
        }else if(event.Key().sym == SDLK_RIGHT) {
            if(cp_ < static_cast<int>(line_.size())) cp_++;
        }
    }else if(event.type == TEXTINPUT && cp_ <= static_cast<int>(line_.size())) {
        char c = event.Get().text.text[0];
        line_.insert(line_.begin() + cp_, c);
        cp_++;
    }
}
