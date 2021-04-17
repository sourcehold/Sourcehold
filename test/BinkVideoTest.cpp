#include <gtest/gtest.h>
#include "Rendering/Display.h"
#include "Rendering/BinkVideo.h"
#include "Audio/Audio.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::FFMPEG;
using namespace Sourcehold::OpenAL;

constexpr auto fps = 30;
constexpr auto ms_per_frame = 1000 / fps;

TEST(BinkVideo, PlayIntro) {
  OpenAL::Init();

  bool quit = false;

  Display::Set("Test BinkVideo", {800, 600}, 0, false, false, false, false);
  auto bik = std::make_unique<BinkStream>(
      "/home/fseidl/projects/Sourcehold/data/binks/intro.bik");
  auto intro = BinkVideo{std::move(bik)};

  Renderer::Clear();
  while (intro.IsRunning() && !quit) {
    auto frame_start = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        quit = true;
      }
    }

    intro.Update();
    Renderer::Draw(intro);
    Renderer::Flush();
    auto frame_time = SDL_GetTicks() - frame_start;
    if (ms_per_frame > frame_time) {
      SDL_Delay(ms_per_frame - frame_time);
    }
  }
  OpenAL::Deinit();
}
