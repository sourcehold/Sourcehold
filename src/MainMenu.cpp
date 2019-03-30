#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man)
{
}

MainMenu::~MainMenu() {

}

void MainMenu::PlayMusic() {
    aud_startup.LoadSong("data/fx/music/stainedglass1.raw");
    aud_startup.Play();
}

int MainMenu::Startup() {
    aud_chantloop.LoadSong("data/fx/music/chantloop1.raw", true);

    tgx_firefly = manager->GetTgx(manager->GetDirectory() + "gfx/logo1.tgx");
    tgx_taketwo = manager->GetTgx(manager->GetDirectory() + "gfx/logo2.tgx");
    tgx_present = manager->GetTgx(manager->GetDirectory() + "gfx/logo3.tgx");
    tgx_logo = manager->GetTgx(manager->GetDirectory() + "gfx/startup screen.tgx");
    intro = manager->GetBik(manager->GetDirectory() + "binks/intro.bik");

    startTime = manager->GetTime();

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
        if(current < STARTUP_STRONGHOLD_LOGO) {
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
        if(current == STARTUP_FIREFLY_LOGO) {
            tgx_firefly->SetAlphaMod(alpha);
            manager->Render(*tgx_firefly);
        }else if(current == STARTUP_TAKETWO_LOGO) {
            tgx_taketwo->SetAlphaMod(alpha);
            manager->Render(*tgx_taketwo);
        }else if(current == STARTUP_PRESENT) {
            tgx_present->SetAlphaMod(alpha);
            manager->Render(*tgx_present);
        }else if(current == STARTUP_STRONGHOLD_LOGO) {
            tgx_logo->SetAlphaMod(alpha);
            manager->Render(*tgx_logo);
        }else if(current == STARTUP_INTRO) {
            aud_startup.SetFadeOut(1.0);
            aud_startup.UpdateFade();
            intro->Update();
            manager->Render(*intro, 0.0, 0.2, 1.0, 0.6);
            if(!intro->IsRunning()) current++;
        }else if(current == STARTUP_MAIN_MENU) {
            aud_startup.Stop();
            return EXIT_SUCCESS;
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
    tgx_bg1 = manager->GetTgx("frontend_main.tgx");

    aud_chantloop.Play();

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg1);

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}