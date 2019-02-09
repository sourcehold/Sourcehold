#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Sound;

MainMenu::MainMenu(GameManager &manager) : GameManager(manager) {
    Context ctx = CreateContext();

    /* Preload images */
    tgx_loading.SetContext(ctx);
    tgx_loading.LoadFromDisk("data/gfx/frontend_loading.tgx");

    tgx_firefly.SetContext(ctx);
    tgx_firefly.LoadFromDisk("data/gfx/frontend_loading.tgx");

    tgx_background.SetContext(ctx);
    tgx_background.LoadFromDisk("data/gfx/frontend_loading.tgx");
}

MainMenu::~MainMenu() {

}

bool MainMenu::Start() {
    AudioSource song = LoadSong("data/fx/music/stainedglass1.raw", true);
    PlayAudio(song);

    while(IsOpen()) {
        Clear();
        HandleEvents();

        RenderTextureFullscreen(tgx_loading);

        Flush();
    }

    return true;
}
