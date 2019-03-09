#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    tgx_loading(man),
    tgx_bg1(man),
    tgx_firefly(man),
    tgx_taketwo(man),
    tgx_present(man),
    tgx_logo(man),
    intro(man)
{

}

MainMenu::~MainMenu() {

}

int MainMenu::Startup() {
    aud_startup.LoadSong("data/fx/music/stainedglass1.raw");
    aud_chantloop.LoadSong("data/fx/music/chantloop1.raw", true);

    tgx_loading.LoadFromDisk("data/gfx/frontend_loading.tgx");
    tgx_bg1.LoadFromDisk("data/gfx/frontend_main.tgx");
    tgx_firefly.LoadFromDisk("data/gfx/logo1.tgx");
    tgx_taketwo.LoadFromDisk("data/gfx/logo2.tgx");
    tgx_present.LoadFromDisk("data/gfx/logo3.tgx");
    tgx_logo.LoadFromDisk("data/gfx/startup screen.tgx");
    intro.LoadFromDisk("data/binks/intro.bik");

    startTime = manager->GetTime();

    aud_startup.Play();

    Uint8 alpha = 255;
    double fadeBase = startTime;
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        /* Logo switching */
        double now = manager->GetTime();
        double delta = now - startTime; 
        if(current != STARTUP_MAIN_MENU && current != STARTUP_STRONGHOLD_LOGO && current != STARTUP_INTRO && delta > 5.0) {
            current++;
            startTime = now;
        }

        /* Logo fading */
        if(current < STARTUP_STRONGHOLD_LOGO && current != STARTUP_LOADING) {
            if(now < fadeBase + 1.0) {
                alpha = Uint8(((now - fadeBase) * 255.0) / 1.0);
            }else if(now < fadeBase + 4.0) {
                alpha = 255;
            }else if(now < fadeBase + 5.0) {
                alpha = 255 - Uint8(((now - (fadeBase + 4.0)) * 255.0) / 1.0);
            }else if(now > fadeBase + 5.0) {
                alpha = 0;
                fadeBase = now;
            }
        }else alpha = 255;

        /* Rendering */
        if(current == STARTUP_LOADING) {
            manager->Render(tgx_loading);
        }else if(current == STARTUP_FIREFLY_LOGO) {
            tgx_firefly.SetAlphaMod(alpha);
            manager->Render(tgx_firefly);
        }else if(current == STARTUP_TAKETWO_LOGO) {
            tgx_taketwo.SetAlphaMod(alpha);
            manager->Render(tgx_taketwo);
        }else if(current == STARTUP_PRESENT) {
            tgx_present.SetAlphaMod(alpha);
            manager->Render(tgx_present);
        }else if(current == STARTUP_STRONGHOLD_LOGO) {
            tgx_logo.SetAlphaMod(alpha);
            manager->Render(tgx_logo);
        }else if(current == STARTUP_INTRO) {
            aud_startup.SetFadeOut(1.0);
            aud_startup.UpdateFade();
            intro.Decode();
            manager->Render(intro, 0.0, 0.1, 1.0, 0.8);
        }else if(current == STARTUP_MAIN_MENU) {
            return EnterMainMenu();
        }
		
        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}

void MainMenu::onEventReceive(Mouse &event) {
    if(event.LmbDown()) {
        if(current != STARTUP_MAIN_MENU) current++;
    }
}

int MainMenu::EnterMainMenu() {
    aud_chantloop.Play();

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(tgx_bg1);

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}
