#include <GUI.h>

#include <System/Logger.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

GUI::GUI(std::shared_ptr<GameManager> man) :
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    mainMenu(man),
    combatMenu(man),
    builderMenu(man),
    economicsMenu(man)
{
}

GUI::~GUI() {

}

void GUI::PlayMusic() {
    aud_startup.LoadSong(manager->GetDirectory() / "/fx/music/stainedglass1.raw");
    aud_startup.Play();
}

int GUI::Startup() {
    aud_chantloop.LoadSong(manager->GetDirectory() / "fx/music/chantloop1.raw", true);

    tgx_firefly = manager->GetTgx(manager->GetDirectory() / "gfx/logo1.tgx").lock();
    tgx_taketwo = manager->GetTgx(manager->GetDirectory() / "gfx/logo2.tgx").lock();
    tgx_present = manager->GetTgx(manager->GetDirectory() / "gfx/logo3.tgx").lock();
    tgx_logo = manager->GetTgx(manager->GetDirectory() / "gfx/startup screen.tgx").lock();
    tgx_firefly_front = manager->GetTgx(manager->GetDirectory() / "gfx/front_firefly_logo.tgx").lock();

    intro = manager->GetBik(manager->GetDirectory() / "binks/intro.bik").lock();

    startTime = manager->GetTime();


    Uint8 alpha = 255;
    double fadeBase = startTime;
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        if(currentUIState == INTRO_SEQUENCE){
            /* Logo switching */
            double now = manager->GetTime();
            double delta = now - startTime;

            /* Logo fading */
            if(currentStartupState < STARTUP_STRONGHOLD_LOGO) {
                if(delta > 5.0) {
                    currentStartupState++;
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
            } else alpha = 255;

            /* Rendering */
            if(currentStartupState == STARTUP_FIREFLY_LOGO) {
                tgx_firefly->SetAlphaMod(alpha);
                manager->Render(*tgx_firefly);
            }else if(currentStartupState == STARTUP_TAKETWO_LOGO) {
                tgx_taketwo->SetAlphaMod(alpha);
                manager->Render(*tgx_taketwo);
            }else if(currentStartupState == STARTUP_PRESENT) {
                tgx_present->SetAlphaMod(alpha);
                manager->Render(*tgx_present);
            }else if(currentStartupState == STARTUP_STRONGHOLD_LOGO) {
                tgx_logo->SetAlphaMod(alpha);
                manager->Render(*tgx_logo);
            }else if(currentStartupState == STARTUP_INTRO) {
                aud_startup.SetFadeOut(1.0);
                aud_startup.UpdateFade();
                intro->Update();
                manager->Render(*intro, 0.0, 0.2, 1.0, 0.6);
                if(!intro->IsRunning()) {
                    currentUIState = MAIN_MENU;
                }
            }else{
                aud_startup.Stop();
                currentUIState = MAIN_MENU;
            }
        } else if(currentUIState == MAIN_MENU) {
            if(!aud_chantloop.IsPlaying()) aud_chantloop.Play();
            currentUIState = mainMenu.EnterMenu();
        } else if(currentUIState == COMBAT_MENU) {
            currentUIState = combatMenu.EnterMenu();
        } else if(currentUIState == BUILDER_MENU) {
            currentUIState = builderMenu.EnterMenu();
        } else if(currentUIState == ECONOMICS_MENU) {
            currentUIState = economicsMenu.EnterMenu();
        } else if (currentUIState == LOAD_SAVED_MENU) {

        } else if(currentUIState == MILITARY_CAMPAIGN_MENU) {

        } else if(currentUIState == SIEGE_MENU) {

        } else if(currentUIState == INVASION_MENU) {

        } else if (currentUIState == ECONOMICS_CAMPAIGN_MENU) {

        } else if(currentUIState == ECONOMICS_MISSION_MENU) {

        } else if(currentUIState == FREE_BUILD_MENU) {

        } else if(currentUIState == WORKING_MAP_MENU) {

        } else if (currentUIState == STAND_ALONE_MISSION_MENU) {

        }  else if(currentUIState == SIEGE_THAT_MENU) {

        } else if(currentUIState == MULTIPLAYER_MAP_MENU) {

        } else if(currentUIState == SETTINGS_MENU) {

        } else if (currentUIState == EXIT_GAME) {
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
        manager->Flush();
        manager->EndTimer();
    }
    return EXIT_SUCCESS;
}

void GUI::onEventReceive(Mouse &event) {
    if(event.LmbDown()) {
        if(currentUIState == INTRO_SEQUENCE) currentStartupState++;
    }
}
