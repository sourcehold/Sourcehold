#include "Startup.h"
#include "Rendering/Font.h"

#include "System/Logger.h"
#include "System/Config.h"

using namespace Sourcehold::Game;
using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Startup::Startup() : EventConsumer<Mouse>() {
}

Startup::~Startup() {
}

void Startup::PlayMusic() {
  aud_startup.Load(GetDirectory() / "fx/music/stainedglass1.raw");
  aud_startup.Play();
}

UIState Startup::Begin() {
  tgx_firefly = GetTgx("gfx/logo1.tgx");
  tgx_taketwo = GetTgx("gfx/logo2.tgx");
  tgx_present = GetTgx("gfx/logo3.tgx");
  tgx_logo = GetTgx("gfx/startup screen.tgx");
  tgx_firefly_front = GetTgx("gfx/front_firefly_logo.tgx");

  intro = GetBik("binks/intro.bik");

  const std::wstring &startupStr = GetString(T_START_TEXT, 1);
  auto font = GetGm1("gm/font_stronghold_aa.gm1")->GetTextureAtlas();
  auto dim = GetStringPixelDim(startupStr, FONT_LARGE);
  dim.first *= 0.7;
  dim.second *= 0.7;

  Uint8 alpha = 255;
  startTime = GetTime();
  double fadeBase = startTime;
  Resolution res = GetResolution();
  StrongholdEdition ed = GetEdition();

  while (Running()) {
    ClearDisplay();

    if (currentUIState == INTRO_SEQUENCE) {
      /* Logo switching */
      double now = GetTime();
      double delta = now - startTime;

      /* Logo fading */
      if (currentStartupState < STARTUP_STRONGHOLD_LOGO ||
          currentStartupState == STARTUP_MULTIPLAYER_INFO) {
        if (delta > 5.0) {
          currentStartupState++;
          startTime = now;
        }
        if (now < fadeBase + 1.0) {
          alpha = Uint8(((now - fadeBase) * 255.0) / 1.0);
        }
        else if (now < fadeBase + 4.0) {
          alpha = 255;
        }
        else if (now < fadeBase + 5.0) {
          alpha = 255 - Uint8(((now - (fadeBase + 4.0)) * 255.0) / 1.0);
        }
        else if (now > fadeBase + 5.0) {
          alpha = 0;
          fadeBase = now;
        }
      }
      else
        alpha = 255;

      /**
       * Images get scaled down on 800x600 and
       * centered but not scaled on every other res
       */
      int px = (GetWidth() / 2) - (1024 / 2);
      int py = (GetHeight() / 2) - (768 / 2);

#if RENDER_LOADING_BORDER == 1
      if (ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
        RenderMenuBorder();
        RenderRect(Rect<int>{px, py, 1024, 768}, 0, 0, 0, 255, true);
      }
#endif

      switch (currentStartupState) {
        case STARTUP_FIREFLY_LOGO:
          tgx_firefly->SetAlphaMod(alpha);
          if (res == RESOLUTION_800x600) {
            Render(*tgx_firefly);
          }
          else {
            Render(*tgx_firefly, px, py);
          }
          break;
        case STARTUP_TAKETWO_LOGO:
          tgx_taketwo->SetAlphaMod(alpha);
          if (res == RESOLUTION_800x600) {
            Render(*tgx_taketwo);
          }
          else {
            Render(*tgx_taketwo, px, py);
          }
          break;
        case STARTUP_PRESENT:
          tgx_present->SetAlphaMod(alpha);
          if (res == RESOLUTION_800x600) {
            Render(*tgx_present);
          }
          else {
            Render(*tgx_present, px, py);
          }
          break;
        case STARTUP_STRONGHOLD_LOGO:
          tgx_logo->SetAlphaMod(alpha);
          if (res == RESOLUTION_800x600) {
            Render(*tgx_logo);
          }
          else {
            Render(*tgx_logo, px, py);
          }
          break;
        case STARTUP_MULTIPLAYER_INFO:
          if (ed != STRONGHOLD_CLASSIC) {
            font->SetAlphaMod(alpha);
            RenderText(startupStr, (GetWidth() / 2) - (dim.first / 2),
                       (GetHeight() / 2) - (dim.second / 2), FONT_LARGE, false,
                       0.7);
          }
          break;
        case STARTUP_INTRO:
          font->SetAlphaMod(255);

          aud_startup.SetFadeOut(1.0);
          aud_startup.UpdateFade();
          intro->Update();

          px = (GetWidth() / 2) - (640 / 2);
          py = (GetHeight() / 2) - (230 / 2);

          Render(*intro, px, py);
          if (!intro->IsRunning()) {
            currentUIState = MAIN_MENU;
          }
          break;
        default:
          aud_startup.Stop();
          return MAIN_MENU;
      }
    }

    FlushDisplay();
    SDL_Delay(1);
  }
  return EXIT_GAME;
}

void Startup::onEventReceive(Mouse &event) {
  if (event.LmbDown() && currentUIState == INTRO_SEQUENCE) {
    currentStartupState++;
    if (currentStartupState == STARTUP_MULTIPLAYER_INFO) {
      startTime = GetTime();
    }
  }
}
