#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    /* UI stuff */
    ui_exit(man),
    ui_firefly(man),
    ui_tutorial(man),
    ui_combat(man),
    ui_economic(man),
    ui_mapedit(man),
    ui_load(man)
{
}

MainMenu::~MainMenu() {

}

void MainMenu::PlayMusic() {
    aud_startup.LoadSong(manager->GetDirectory() / "/fx/music/stainedglass1.raw");
    aud_startup.Play();
}

int MainMenu::Startup() {
    aud_chantloop.LoadSong(manager->GetDirectory() / "fx/music/chantloop1.raw", true);

    tgx_firefly = manager->GetTgx(manager->GetDirectory() / "gfx/logo1.tgx").lock();
    tgx_taketwo = manager->GetTgx(manager->GetDirectory() / "gfx/logo2.tgx").lock();
    tgx_present = manager->GetTgx(manager->GetDirectory() / "gfx/logo3.tgx").lock();
    tgx_logo = manager->GetTgx(manager->GetDirectory() / "gfx/startup screen.tgx").lock();
    tgx_firefly_front = manager->GetTgx(manager->GetDirectory() / "gfx/front_firefly_logo.tgx").lock();

    intro = manager->GetBik(manager->GetDirectory() / "binks/intro.bik").lock();

    gm1_icons_front = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end.gm1").lock();

    startTime = manager->GetTime();

    Uint8 alpha = 255;
    double fadeBase = startTime;
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        /* Logo switching */
        double now = manager->GetTime();
        double delta = now - startTime;

        /* Logo fading */
        if(current < STARTUP_STRONGHOLD_LOGO) {
            if(delta > 5.0) {
                current++;
                startTime = now;
            }
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
    tgx_bg1 = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_main2.tgx").lock();

    /* Init user interface */
    InitUI();

    aud_chantloop.Play();

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg1);

        /* Render user interface */
        auto icons_front = gm1_icons_front->GetTextureAtlas().lock();
        ui_exit.Render(
            [&]() -> Texture& {
                if(ui_exit.IsMouseOver()) icons_front->SetRect(icons_front->Get(69));
                else icons_front->SetRect(icons_front->Get(68));
                return *icons_front;
            });

        ui_tutorial.Render(
            [&]() -> Texture& {
                if(ui_tutorial.IsMouseOver()) icons_front->SetRect(icons_front->Get(73));
                else icons_front->SetRect(icons_front->Get(72));
                return *icons_front;
            });

        ui_combat.Render(
            [&]() -> Texture& {
                if(ui_combat.IsMouseOver()) {
                    icons_front->SetRect(icons_front->Get(16));
                    ui_combat.Translate(0.178, 0.279);
                } else {
                    icons_front->SetRect(icons_front->Get(0));
                    ui_combat.Translate(0.178, 0.278);
                }
                return *icons_front;
            });

        ui_economic.Render(
            [&]() -> Texture& {
                if(ui_economic.IsMouseOver()) {
                    icons_front->SetRect(icons_front->Get(33));
                    ui_economic.Translate(0.333, 0.279);
                } else {
                    icons_front->SetRect(icons_front->Get(17));
                    ui_economic.Translate(0.333, 0.278);
                }
                return *icons_front;
            });

        ui_mapedit.Render(
            [&]() -> Texture& {
                if(ui_mapedit.IsMouseOver()) {
                    icons_front->SetRect(icons_front->Get(50));
                    ui_mapedit.Translate(0.485, 0.279);
                } else {
                    icons_front->SetRect(icons_front->Get(34));
                    ui_mapedit.Translate(0.485, 0.278);
                }
                return *icons_front;
            });

        ui_load.Render(
            [&]() -> Texture& {
                if(ui_load.IsMouseOver()) {
                    icons_front->SetRect(icons_front->Get(67));
                    ui_load.Translate(0.643, 0.279);
                } else {
                    icons_front->SetRect(icons_front->Get(51));
                    ui_load.Translate(0.643, 0.278);
                }
                return *icons_front;
            });

//        ui_firefly.Render(*tgx_firefly_front, true);

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}

void MainMenu::InitUI() {
    /* Exit button */
    ui_exit.Translate(0.15, 0.8);
    ui_exit.Scale(0.15, 0.2);

    /* 'Start credits' button */
    ui_firefly.Translate(0.4, 0.8);
    ui_firefly.Scale(0.2, 0.2);

    /* 'Start tutorial' button */
    ui_tutorial.Translate(0.65, 0.78);
    ui_tutorial.Scale(0.2, 0.24);

    ui_combat.Scale(0.185, 0.232);
    ui_economic.Scale(0.185, 0.232);
    ui_mapedit.Scale(0.185, 0.232);
    ui_load.Scale(0.185, 0.232);
}

