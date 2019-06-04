#include <GUI/Startup.h>

#include <System/Logger.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Startup::Startup(std::shared_ptr<GameManager> man) :
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    mainMenu(man)
{
}

Startup::~Startup() {

}

void Startup::PlayMusic() {
    aud_startup.LoadSong(manager->GetDirectory() / "/fx/music/stainedglass1.raw");
    aud_startup.Play();
}

int Startup::Begin() {
    tgx_firefly = manager->GetTgx(manager->GetDirectory() / "gfx/logo1.tgx").lock();
    tgx_taketwo = manager->GetTgx(manager->GetDirectory() / "gfx/logo2.tgx").lock();
    tgx_present = manager->GetTgx(manager->GetDirectory() / "gfx/logo3.tgx").lock();
    tgx_logo = manager->GetTgx(manager->GetDirectory() / "gfx/startup screen.tgx").lock();
    tgx_firefly_front = manager->GetTgx(manager->GetDirectory() / "gfx/front_firefly_logo.tgx").lock();

    intro = manager->GetBik(manager->GetDirectory() / "binks/intro.bik").lock();

    startTime = manager->GetTime();

    Uint8 alpha = 255;
    double fadeBase = startTime;
	Resolution res = manager->GetResolution();
    while(manager->Running()) {
        manager->Clear();

        if(currentUIState == INTRO_SEQUENCE) {
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

            /**
			* Images get scaled down on 800x600 and
			* centered but not scaled on every other res
			*/
			int px = (manager->GetWidth() / 2) - (1024 / 2);
			int py = (manager->GetHeight() / 2) - (768 / 2);
            if(currentStartupState == STARTUP_FIREFLY_LOGO) {
                tgx_firefly->SetAlphaMod(alpha);
				if (res == RESOLUTION_800x600) {
					manager->Render(*tgx_firefly);
				}
				else {
					manager->Render(*tgx_firefly, px, py);
				}
            }else if(currentStartupState == STARTUP_TAKETWO_LOGO) {
                tgx_taketwo->SetAlphaMod(alpha);
				if (res == RESOLUTION_800x600) {
					manager->Render(*tgx_taketwo);
				}
				else {
					manager->Render(*tgx_taketwo, px, py);
				}
			}else if(currentStartupState == STARTUP_PRESENT) {
                tgx_present->SetAlphaMod(alpha);
				if (res == RESOLUTION_800x600) {
					manager->Render(*tgx_present);
				}
				else {
					manager->Render(*tgx_present, px, py);
				}
            }else if(currentStartupState == STARTUP_STRONGHOLD_LOGO) {
                tgx_logo->SetAlphaMod(alpha);
				if (res == RESOLUTION_800x600) {
					manager->Render(*tgx_logo);
				}
				else {
					manager->Render(*tgx_logo, px, py);
				}
            }else if(currentStartupState == STARTUP_INTRO) {
                aud_startup.SetFadeOut(1.0);
                aud_startup.UpdateFade();
                intro->Update();

				px = (manager->GetWidth() / 2) - (640 / 2);
				py = (manager->GetHeight() / 2) - (230 / 2);

				manager->Render(*intro, px, py);
                if(!intro->IsRunning()) {
                    currentUIState = MAIN_MENU;
                }
            }else{
                aud_startup.Stop();
                currentUIState = MAIN_MENU;
            }
        } else if(currentUIState == MAIN_MENU) {
            currentUIState = mainMenu.EnterMenu();
        }else if (currentUIState == EXIT_GAME) {
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
        manager->Flush();
        manager->Sync();
    }
    return EXIT_FAILURE;
}

void Startup::onEventReceive(Mouse &event) {
    if(event.LmbDown()) {
        if(currentUIState == INTRO_SEQUENCE) currentStartupState++;
    }
}
