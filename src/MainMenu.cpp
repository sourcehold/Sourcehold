#include <MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;

MainMenu::MainMenu(GameManager &manager) : GameManager(manager) {
    /* Preload images */
/*    tgx_loading.LoadFromDisk("data/gfx/frontend_loading.tgx");
    tgx_firefly.LoadFromDisk("data/gfx/front_firefly_logo.tgx");
    tgx_background.LoadFromDisk("data/gfx/frontend_main2.tgx");
    gm1_slider_bar.LoadFromDisk("data/gm/interface_slider_bar.gm1");*/
}

MainMenu::~MainMenu() {

}

bool MainMenu::Start() {
    //AudioSource song = LoadSong("data/fx/music/stainedglass1.raw", true);
    //PlayAudio(song);

    while(IsOpen()) {
        Clear();
        HandleEvents();
        StartTimer();

        //RenderTextureFullscreen(tgx_loading);
        //RenderTextureScale(gm1_slider_bar.GetEntry(0).image, 0.1, 0.7, 0.8, 0.1);

        Flush();
        EndTimer();
    }

    return true;
}
