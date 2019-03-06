#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    EventConsumer<Keyboard>(man->GetHandler()),
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
    AudioSource aud("data/fx/music/sadtimesa.raw");
    aud.Play();

    tgx_loading.LoadFromDisk("data/gfx/frontend_loading.tgx");
    tgx_bg1.LoadFromDisk("data/gfx/frontend_main2.tgx");
    tgx_firefly.LoadFromDisk("data/gfx/logo1.tgx");
    tgx_taketwo.LoadFromDisk("data/gfx/logo2.tgx");
    tgx_present.LoadFromDisk("data/gfx/logo3.tgx");
    tgx_logo.LoadFromDisk("data/gfx/startup screen.tgx");
    intro.LoadFromDisk("data/binks/intro.bik");

    startTime = manager->GetTime();

	AniFile ani;
	ani.LoadFromDisk("data/sword.ani");
	
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        double delta = manager->GetTime() - startTime;

        if(delta < 15.0) {
            manager->Render(tgx_loading);
        }else if(delta < 20.0) {
            manager->Render(tgx_firefly);
        }else if(delta < 25.0) {
            manager->Render(tgx_taketwo);
        }else if(delta < 28.0) {
            manager->Render(tgx_present);
        }else {
            manager->Render(tgx_logo);
        }
		
        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}

void MainMenu::onEventReceive(Keyboard &event) {

}

void MainMenu::onEventReceive(Mouse &event) {
    if(event.LmbDown()) {
        introPlaying = false;
        startupScreen = false;
    }
}
