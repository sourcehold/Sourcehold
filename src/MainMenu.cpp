#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    manager(man),
    EventHandler<Keyboard>(),
    tgx_loading(man),
    tgx_bg1(man),
    intro(man)
{

}

MainMenu::~MainMenu() {

}

int MainMenu::Startup() {
    tgx_loading.LoadFromDisk("data/gfx/frontend_loading.tgx");
    tgx_bg1.LoadFromDisk("data/gfx/frontend_main2.tgx");
    intro.LoadFromDisk("data/binks/intro.bik");

    while(manager->Running()) {
        manager->Clear();
        manager->HandleEvents();
        manager->StartTimer();

        intro.Decode();
        manager->RenderFullscreen(intro);

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_SUCCESS;
}
