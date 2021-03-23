#include "GUI/Widgets/LineEdit.h"
#include "Rendering/Font.h"
#include "GameManager.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;

LineEdit::LineEdit(int nx, const std::wstring& line)
    : EventConsumer<Keyboard>(), line{line}, nx{nx}, cp{0} {
}

LineEdit::~LineEdit() {
}

void LineEdit::Init() {
  cp = static_cast<int>(line.size());
}

void LineEdit::BeginInput() {
  SDL_StartTextInput();
}

void LineEdit::EndInput() {
  SDL_StopTextInput();
}

void LineEdit::Update(Rect<int> constraints) {
  std::shared_ptr<TextureAtlas> atlas =
      GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();
  SDL_Rect parts[3];
  parts[0] = atlas->Get(114);
  parts[1] = atlas->Get(115);
  parts[2] = atlas->Get(116);

  auto dim = GetStringPixelDim(line.substr(0, cp), FONT_SMALL);

  int x = constraints.x + ((constraints.w - LINE_SEGMENT_W * nx) / 2);
  int y = constraints.y + ((constraints.h - parts[0].h) / 2);

  atlas->SetAlphaMod(127);

  ::Render(*atlas, x, y, &parts[0]);
  ::Render(*atlas, x + LINE_SEGMENT_W * nx, y, &parts[2]);

  for (int px = LINE_SEGMENT_W; px < nx * LINE_SEGMENT_W;
       px += LINE_SEGMENT_W) {
    ::Render(*atlas, x + px, y, &parts[1]);
  }

  atlas->SetAlphaMod(255);

  // Text contents
  RenderText(line, x + 4, y + 6, FONT_SMALL);

  // Cursor
  RenderRect({x + static_cast<int>(dim.first), y, 2, 34},  //
             24, 80, 24, 255, true);
}

void LineEdit::onEventReceive(Keyboard& event) {
  if (event.type == KEYDOWN) {
    if (event.Key().sym == SDLK_BACKSPACE) {
      if (line.size() >= 1 && cp) {
        line.erase(line.begin() + cp - 1);
        cp--;
      }
    }
    else if (event.Key().sym == SDLK_DELETE) {
      if (cp < line.size()) {
        line.erase(line.begin() + cp);
      }
    }
    else if (event.Key().sym == SDLK_LEFT) {
      if (cp)
        cp--;
    }
    else if (event.Key().sym == SDLK_RIGHT) {
      if (cp < line.size())
        cp++;
    }
  }
  else if (event.type == TEXTINPUT && cp <= line.size()) {
    char c = event.Get().text.text[0];
    line.insert(line.begin() + cp, c);
    cp++;
  }
}
