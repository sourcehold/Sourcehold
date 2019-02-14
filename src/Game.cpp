#include <Game.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Sound;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;

Game::Game() : GameManager(), menu(*this) {

}

Game::~Game() {
    
}

bool Game::Init(CmdLineOptions &opt) {
    //MlbFile mlb;
    //mlb.LoadFromDisk(boost::filesystem::path("data/stronghold.mlb"));

    Context ctx = GameManager::CreateContext();

    Gm1File gm1;
    gm1.SetContext(ctx);
    if(!gm1.LoadFromDisk("data/gm/tile_castle.gm1")) {
        Logger::error("GAME") << "Gm1 file loading error!" << std::endl;
    }

    auto entries = gm1.GetEntries();
    tex1 = entries[80].image;
    //tex2 = entries[1].image;
    //tex3 = entries[2].image;
    //tex4 = entries[3].image;

    tgx.SetContext(ctx);
    if(!tgx.LoadFromDisk("data/gfx/frontend_loading.tgx")) {
        Logger::error("GAME") << "Tgx file loading error!" << std::endl;
    }

    //VolumeTxt txt;
    //txt.LoadFromDisk("data/fx/volume.txt");

    //bik.Init(ctx);
    //bik.LoadFromDisk("data/binks/intro.bik");

    return true;
}

int Game::Start() {
    /*if(!menu.Start()) {
        // exited out of intro or menu
        return EXIT_SUCCESS;
    }*/

    AudioSource song = LoadSong("data/fx/music/appytimes.raw", true);
    //PlayAudio(song);

    while(Display::IsOpen()) {
        Display::Clear();
        Display::HandleEvents();
        Display::StartTimer();

        Display::RenderTextureFullscreen(tgx);
        Display::RenderTexture(tex1, 0, 0);
        //Display::RenderTextureScale(tex1, 0, 0, 30*8, 16*8);
        //Display::RenderTextureScale(tex2, 30*8, 0, 30*8, 16*8);
        //Display::RenderTextureScale(tex3, 0, 16*8, 30*8, 16*8);
        //Display::RenderTextureScale(tex4, 30*8, 16*8, 30*8, 16*8);

        //Texture &frame = bik.RenderFrame();
        //Display::RenderTexture(frame, 0, 0);

        Display::Flush();
        Display::EndTimer();
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    Game game;
    CmdLineOptions opt = ParseCmdLine(argc, argv);
    if(!game.Init(opt)) {
        Logger::error("GAME") << "Game initialization failed due to previous errors!" << std::endl;
        return EXIT_FAILURE;
    }

    return game.Start();
}

#if SOURCEHOLD_WINDOWS == 1
#include <windows.h>
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow) {
    return main(NULL, 0);
}
#endif
