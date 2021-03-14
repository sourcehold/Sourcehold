#pragma once

#include <memory>
#include <cinttypes>
#include <vector>
#include <string>

#include "Events/Event.h"
#include "Events/Mouse.h"

#include "Audio/Song.h"

#include "Parsers/TgxFile.h"
#include "Assets.h"

/* Narration screens shown before campaign missions */
namespace Sourcehold {
namespace GUI {
using Parsers::TgxFile;

using namespace Events;
using namespace Assets;
using namespace Audio;

enum class NarrBackground { GOODGUYS, BADGUYS };

#define NUM_ANIM_FRAMES 13
class NarrScreen : protected EventConsumer<Mouse> {
  std::vector<std::wstring> lines;
  Song song;
  bool skipped;
  uint8_t mission;
  std::shared_ptr<TgxFile> tgx_bg1, tgx_bg2, tgx_candle[NUM_ANIM_FRAMES],
      tgx_flame[2][NUM_ANIM_FRAMES];

 public:
  /* Create sequence based on mission index */
  NarrScreen(uint8_t mission);
  ~NarrScreen();

  bool Begin();

 protected:
  void onEventReceive(Mouse &mouse) override;

  bool BeginAct(TextSection text);
  bool BeginNarration();
  bool BeginStoryScreen(NarrBackground bg);
  bool BeginNpcIntro(NPC npc);

  void RenderFlameAnim(int px, int py, int index, Uint8 alpha);
  void NarrationText(TextSection text);
};
}  // namespace GUI
}  // namespace Sourcehold
