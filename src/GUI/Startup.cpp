#include <GUI/Startup.h>

#include <System/Logger.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Startup::Startup() :
    EventConsumer<Mouse>()
{
}

Startup::~Startup()
{

}

void Startup::PlayMusic()
{
    aud_startup.LoadSong(GetDirectory() / "/fx/music/stainedglass1.raw");
    aud_startup.Play();
}

int Startup::Begin()
{
    tgx_firefly = GetTgx(GetDirectory() / "gfx/logo1.tgx");
    tgx_taketwo = GetTgx(GetDirectory() / "gfx/logo2.tgx");
    tgx_present = GetTgx(GetDirectory() / "gfx/logo3.tgx");
    tgx_logo = GetTgx(GetDirectory() / "gfx/startup screen.tgx");
    tgx_firefly_front = GetTgx(GetDirectory() / "gfx/front_firefly_logo.tgx");

    intro = GetBik(GetDirectory() / "binks/intro.bik");

    startTime = GetTime();

    Uint8 alpha = 255;
    double fadeBase = startTime;
    Resolution res = GetResolution();
    StrongholdEdition ed = GetEdition();

    while(Running()) {
        ClearDisplay();

        if(currentUIState == INTRO_SEQUENCE) {
            /* Logo switching */
            double now = GetTime();
            double delta = now - startTime;

            /* Logo fading */
            if(currentStartupState < STARTUP_STRONGHOLD_LOGO) {
                if(delta > 5.0) {
                    currentStartupState++;
                    startTime = now;
                }
                if(now < fadeBase + 1.0) {
                    alpha = Uint8(((now - fadeBase) * 255.0) / 1.0);
                }
                else if(now < fadeBase + 4.0) {
                    alpha = 255;
                }
                else if(now < fadeBase + 5.0) {
                    alpha = 255 - Uint8(((now - (fadeBase + 4.0)) * 255.0) / 1.0);
                }
                else if(now > fadeBase + 5.0) {
                    alpha = 0;
                    fadeBase = now;
                }
            }
            else alpha = 255;

            /**
            * Images get scaled down on 800x600 and
            * centered but not scaled on every other res
            */
            int px = (GetWidth() / 2) - (1024 / 2);
            int py = (GetHeight() / 2) - (768 / 2);

#if RENDER_LOADING_BORDER == 1
            if(ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
                RenderMenuBorder();
                RenderRect(px, py, 1024, 768, 0, 0, 0, 255, true);
            }
#endif

            if(currentStartupState == STARTUP_FIREFLY_LOGO) {
                tgx_firefly->SetAlphaMod(alpha);
                if (res == RESOLUTION_800x600) {
                    Render(*tgx_firefly);
                }
                else {
                    Render(*tgx_firefly, px, py);
                }
            }
            else if(currentStartupState == STARTUP_TAKETWO_LOGO) {
                tgx_taketwo->SetAlphaMod(alpha);
                if (res == RESOLUTION_800x600) {
                    Render(*tgx_taketwo);
                }
                else {
                    Render(*tgx_taketwo, px, py);
                }
            }
            else if(currentStartupState == STARTUP_PRESENT) {
                tgx_present->SetAlphaMod(alpha);
                if (res == RESOLUTION_800x600) {
                    Render(*tgx_present);
                }
                else {
                    Render(*tgx_present, px, py);
                }
            }
            else if(currentStartupState == STARTUP_STRONGHOLD_LOGO) {
                tgx_logo->SetAlphaMod(alpha);
                if (res == RESOLUTION_800x600) {
                    Render(*tgx_logo);
                }
                else {
                    Render(*tgx_logo, px, py);
                }
            }
            else if(currentStartupState == STARTUP_INTRO) {
                aud_startup.SetFadeOut(1.0);
                aud_startup.UpdateFade();
                intro->Update();

                px = (GetWidth() / 2) - (640 / 2);
                py = (GetHeight() / 2) - (230 / 2);

                Render(*intro, px, py);
                if(!intro->IsRunning()) {
                    currentUIState = MAIN_MENU;
                }
            }
            else {
                aud_startup.Stop();
                currentUIState = MAIN_MENU;
            }
        }
        else if(currentUIState == MAIN_MENU) {
            currentUIState = mainMenu.EnterMenu();
        }
        else if (currentUIState == EXIT_GAME) {
            return EXIT_FAILURE;
        }
        else {
            return EXIT_SUCCESS;
        }
        FlushDisplay();
        SyncDisplay();
    }
    return EXIT_FAILURE;
}

void Startup::onEventReceive(Mouse &event)
{
    if(event.LmbDown()) {
        if(currentUIState == INTRO_SEQUENCE) currentStartupState++;
    }
}
