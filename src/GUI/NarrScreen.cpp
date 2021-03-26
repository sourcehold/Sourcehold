#include "GUI/NarrScreen.h"
#include "GUI/MenuUtils.h"

#include "Rendering/Font.h"
#include "Rendering/BinkVideo.h"

#include "Parsers/Gm1File.h"
#include "GameManager.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

enum class NarrPart {
  NARRATION,    // the candle + scrolling text
  STORYSCREEN,  // the fireplace / castle + npc dialogue
  NPCINTRO      // the introduction to an NPC (snake, wolf, etc.)
};

const char *anim_paths[NUM_ANIM_FRAMES * 3] = {
    // Flame color //
    "flame_01",
    "flame_02",
    "flame_03",
    "flame_04",
    "flame_05",
    "flame_06",
    "flame_07",
    "flame_08",
    "flame_09",
    "flame_10",
    "flame_11",
    "flame_12",
    "flame_13",
    // Flame alpha //
    "flame_alpha_01",
    "flame_alpha_02",
    "flame_alpha_03",
    "flame_alpha_04",
    "flame_alpha_05",
    "flame_alpha_06",
    "flame_alpha_07",
    "flame_alpha_08",
    "flame_alpha_09",
    "flame_alpha_10",
    "flame_alpha_11",
    "flame_alpha_12",
    "flame_alpha_13",
    // Candle //
    "candle_01",
    "candle_02",
    "candle_03",
    "candle_04",
    "candle_05",
    "candle_06",
    "candle_07",
    "candle_08",
    "candle_09",
    "candle_10",
    "candle_11",
    "candle_12",
    "candle_13",
};

NarrScreen::NarrScreen(uint8_t mission)
    : mission(mission), EventConsumer<Mouse>(), skipped(false) {
  std::string path =
      std::string("fx/music/mandloop") + (mission % 2 == 0 ? "1.raw" : "2.raw");

  song.Load(GetDirectory() / path, true);

  tgx_bg1 = GetTgx("gfx/narrbase.tgx");

  // Load animation frames //
  for (uint8_t i = 0; i < NUM_ANIM_FRAMES; i++) {
    const std::string prefix("gfx/");
    const std::string suffix(".tgx");

    // Flame //
    tgx_flame[0][i] = GetTgx(prefix + anim_paths[i] + suffix);  // color
    tgx_flame[1][i] =
        GetTgx(prefix + anim_paths[i + NUM_ANIM_FRAMES] + suffix);  // alpha

    // Candle //
    tgx_candle[i] =
        GetTgx(prefix + anim_paths[i + NUM_ANIM_FRAMES * 2] + suffix);  // color
  }
}

NarrScreen::~NarrScreen() {
}

bool NarrScreen::Begin() {
  song.Play();
  switch (mission) {
    // TODO: add all campaign missions
    case 1:
      if (!BeginAct(T_START_ACT_ONE))
        break;
      if (!BeginNarration())
        break;
      if (!BeginStoryScreen(NarrBackground::GOODGUYS))
        break;
      break;
    default:
      break;
  }
  song.Stop();

  return Running();
}

void NarrScreen::onEventReceive(Mouse &mouse) {
  if (mouse.type == BUTTONDOWN) {
    skipped = true;
  }
}

bool NarrScreen::BeginAct(TextSection text) {
  Uint8 alpha = 255;
  const std::wstring &str = GetString(text, 2);
  auto font = GetGm1("gm/font_stronghold_aa.gm1")->GetTextureAtlas();
  auto dim = GetStringPixelDim(str, FONT_LARGE);

  int px = (GetWidth() / 2) - (dim.first / 2);
  int py = (GetHeight() / 2) - (dim.second / 2);

  double startTime = GetTime();
  while (Running()) {
    if (skipped) {
      skipped = false;
      font->SetAlphaMod(255);
      break;
    }

    double now = GetTime();
    double delta = now - startTime;

    if (delta > 5.0) {
      break;
    }
    if (now < startTime + 1.0) {
      alpha = Uint8(((now - startTime) * 255.0) / 1.0);
    }
    else if (now < startTime + 4.0) {
      alpha = 255;
    }
    else if (now < startTime + 5.0) {
      alpha = 255 - Uint8(((now - (startTime + 4.0)) * 255.0) / 1.0);
    }

    Renderer::Instance().Clear();

    font->SetAlphaMod(alpha);
    RenderText(str, px, py, FONT_LARGE, false);

    Renderer::Instance().Flush();
  }

  font->SetAlphaMod(255);
  return Running();
}

bool NarrScreen::BeginNarration() {
  Resolution res = GetResolution();
  StrongholdEdition ed = GetEdition();

  int px = (GetWidth() / 2) - (1024 / 2);
  int py = (GetHeight() / 2) - (768 / 2);

  Uint8 alpha = 0;
  double fadeBase = GetTime();
  while (Running()) {
    if (skipped) {
      skipped = false;
      tgx_bg1->SetAlphaMod(255);
      break;
    }

    double now = GetTime();
    double delta = now - fadeBase;

    if (delta > 2.)
      alpha = 255;
    else
      alpha = Uint8(((now - fadeBase) * 255.0) / 2.0);

    tgx_bg1->SetAlphaMod(alpha);

    Renderer::Instance().Clear();

    if (ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
      RenderMenuBorder();
      Renderer::Instance().RenderRect(Rect<int>{px, py, 1024, 768},
                                      {0, 0, 0, 255}, true);
    }

    int index = 1 + (11 - abs(int(GetTime() * 15.0) % (2 * 11) - 11));

    Renderer::Instance().Render(*tgx_bg1, px, py);
    RenderFlameAnim(px, py, index, alpha);

    Renderer::Instance().Flush();
  }

  return Running();
}

bool NarrScreen::BeginStoryScreen(NarrBackground bg) {
  BinkVideo bik;
  std::shared_ptr<TgxFile> castle;
  if (bg == NarrBackground::BADGUYS) {
    castle = GetTgx("gfx/storyscreen_castle.tgx");
  }
  else {
    bik.LoadFromDisk(GetDirectory() / "binks/fireplace_01.bik", true);
  }

  int px = (GetWidth() / 2) - (280 / 2);
  int py = (GetHeight() / 2) - (200 / 2);

  while (Running()) {
    if (skipped) {
      skipped = false;
      return true;
    }

    Renderer::Instance().Clear();

    if (bg == NarrBackground::BADGUYS) {
      Renderer::Instance().Render(*castle, px, py);
    }
    else {
      // after deletion it tries to operate on memory
      // which causes crash (tested on windows, not sure about other platforms)
      // result of commenting is that, the movie is not rendering
      // TODO - make issue to track this
      // bik.Update();
      Renderer::Instance().Render(bik, px, py);
    }

    Renderer::Instance().Flush();
  }
  return Running();
}

bool NarrScreen::BeginNpcIntro(NPC npc) {
  return Running();
}

void NarrScreen::RenderFlameAnim(int px, int py, int index, Uint8 a) {
  auto color = tgx_flame[0][index];
  auto alpha = tgx_flame[1][index];
  auto candle = tgx_candle[index];

  color->SetAlphaMod(a);
  candle->SetAlphaMod(a);
  alpha->SetAlphaMod(a);

  alpha->SetBlendMode(SDL_BLENDMODE_ADD);
  Renderer::Instance().Render(*alpha, px + 160, py + 235);

  color->SetBlendMode(SDL_BLENDMODE_ADD);
  Renderer::Instance().Render(*color, px + 160, py + 235);

  Renderer::Instance().Render(*candle, px, py + 393);
}

void NarrScreen::NarrationText(TextSection text) {
  /* Split narration text into lines
     TODO: move into some utils file */

  const std::wstring &str = GetString(text, 3);
}
