#include "GUI/Dialog.h"
#include "Rendering/Font.h"
#include "GameManager.h"

using namespace Sourcehold;
using namespace GUI;
using namespace Game;
using namespace Rendering;

Dialog::~Dialog() {
}
Dialog::Dialog(WidgetLayout l, int nx, int ny, const std::wstring& text,
               bool textbox, Deco deco, int textboxW, bool border)
    : Container(l),
      nx(nx),
      ny(ny),
      tw{textboxW},
      text{text},
      textbox{textbox},
      border{border},
      deco{deco} {
}

void Dialog::Update(Dialog::Position pos, int offX, int offY) {
  int w = nx * MENU_TILE_DIM;
  int h = ny * MENU_TILE_DIM;

  int x = (pos == Dialog::CENTRE) ? ((GetWidth() - w) / 2) + offX : offX;
  int y = (pos == Dialog::CENTRE) ? ((GetHeight() - h) / 2) + offY : offY;

  icons = GetGm1("gm/interface_icons3.gm1");

  if (border)
    RenderBorder(x, y, nx, ny);
  if (textbox)
    RenderTextBox(x, y, tw * 24, 64, text, deco);

  // Render the member widgets in the available area of the dialog //
  w += MENU_TILE_DIM;  // TODO
  h += MENU_TILE_DIM;
  auto screen_space = Rect<int>{x + MARGIN_X,                       //
                                y + MARGIN_Y + (textbox ? 64 : 0),  //
                                w - MARGIN_X * 2,                   //
                                h - MARGIN_Y * 2 - (textbox ? 64 : 0)};
  Container::Update(screen_space);
}

void Dialog::RenderBorder(int x, int y, int nx, int ny) {
  SDL_Rect rect;
  auto atlas = icons->GetTextureAtlas();
  auto& renderer = Renderer::Instance();

  // background
  renderer.RenderRect(
      Rect<int>{x + 8, y + 8, 8 + nx * MENU_TILE_DIM, 8 + ny * MENU_TILE_DIM},
      0, 0, 0, 128, true);

  /* Render alpha masks */
  atlas->SetBlendMode(SDL_BLENDMODE_ADD);

  // corners
  rect = atlas->Get(3);
  renderer.Render(*atlas, x, y, &rect);
  rect = atlas->Get(15);
  renderer.Render(*atlas, x, y + ny * MENU_TILE_DIM, &rect);
  rect = atlas->Get(5);
  renderer.Render(*atlas, x + nx * MENU_TILE_DIM, y, &rect);
  rect = atlas->Get(17);
  renderer.Render(*atlas, x + nx * MENU_TILE_DIM, y + ny * MENU_TILE_DIM,
                  &rect);

  // edges
  for (int ix = x + MENU_TILE_DIM; ix < x + nx * MENU_TILE_DIM;
       ix += MENU_TILE_DIM) {
    rect = atlas->Get(4);
    renderer.Render(*atlas, ix, y, &rect);
    rect = atlas->Get(16);
    renderer.Render(*atlas, ix, y + ny * MENU_TILE_DIM, &rect);
  }
  for (int iy = y + MENU_TILE_DIM; iy < y + ny * MENU_TILE_DIM;
       iy += MENU_TILE_DIM) {
    rect = atlas->Get(9);
    renderer.Render(*atlas, x, iy, &rect);
    rect = atlas->Get(11);
    renderer.Render(*atlas, x + nx * MENU_TILE_DIM, iy, &rect);
  }

  /* Render color */
  atlas->SetBlendMode(SDL_BLENDMODE_MOD);

  // corners
  rect = atlas->Get(0);
  renderer.Render(*atlas, x, y, &rect);
  rect = atlas->Get(12);
  renderer.Render(*atlas, x, y + ny * MENU_TILE_DIM, &rect);
  rect = atlas->Get(2);
  renderer.Render(*atlas, x + nx * MENU_TILE_DIM, y, &rect);
  rect = atlas->Get(14);
  renderer.Render(*atlas, x + nx * MENU_TILE_DIM, y + ny * MENU_TILE_DIM,
                  &rect);

  // edges
  for (int ix = x + MENU_TILE_DIM; ix < x + nx * MENU_TILE_DIM;
       ix += MENU_TILE_DIM) {
    rect = atlas->Get(1);
    renderer.Render(*atlas, ix, y, &rect);
    rect = atlas->Get(13);
    renderer.Render(*atlas, ix, y + ny * MENU_TILE_DIM, &rect);
  }
  for (int iy = y + MENU_TILE_DIM; iy < y + ny * MENU_TILE_DIM;
       iy += MENU_TILE_DIM) {
    rect = atlas->Get(6);
    renderer.Render(*atlas, x, iy, &rect);
    rect = atlas->Get(8);
    renderer.Render(*atlas, x + nx * MENU_TILE_DIM, iy, &rect);
  }

  atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderDeco(Deco type, int x, int y) {
  auto& renderer = Renderer::Instance();

  if (type == Deco::NONE)
    return;

  auto atlas = icons->GetTextureAtlas();

  atlas->SetBlendMode(SDL_BLENDMODE_ADD);

  SDL_Rect rect = atlas->Get(91);
  renderer.Render(*atlas, x, y, &rect);

  atlas->SetBlendMode(SDL_BLENDMODE_MOD);

  rect = atlas->Get(90);
  renderer.Render(*atlas, x, y, &rect);

  atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void Dialog::RenderTextBox(int x, int y, int w, int h, const std::wstring& text,
                           Deco deco) {
  auto& renderer = Renderer::Instance();

  renderer.RenderRect(Rect<int>{x + 8, y + 8, w + 8, h + 8}, 24, 80, 24, 200,
                      true);
  renderer.RenderRect(Rect<int>{x + 8, y + 8, w + 8, h + 8}, 247, 235, 198, 255,
                      false);

  auto dim = GetStringPixelDim(text, FONT_LARGE);
  RenderText(text, x + 20 + (w / 2) - (dim.first / 2), y + 25, FONT_LARGE);

  RenderDeco(deco, x + 11, y + (h / 2) - 11);
  RenderDeco(deco, x + (w - 16), y + (h / 2) - 11);
}
