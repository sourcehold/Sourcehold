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

    /*Tileset set;
    set.SetContext(ctx);
    if(!set.LoadFromDisk("data/gm/tile_castle.gm1")) {
        Logger::error("GAME") << "Tileset loading error!" << std::endl;
    }*/

    //auto entries = set.GetEntries();
    //tex1 = entries[0].image;
    //tex2 = entries[1].image;
    //tex3 = entries[2].image;
    //tex4 = entries[3].image;

    tgx.SetContext(ctx);
    if(!tgx.LoadFromDisk("data/gfx/defeat_pig.tgx")) {
        Logger::error("GAME") << "Tgx file loading error!" << std::endl;
    }

    VolumeTxt txt;
    txt.LoadFromDisk("data/fx/volume.txt");

    bik.Init(ctx);
    if(!bik.LoadFromDisk("data/binks/intro.bik")) {
        Logger::error("GAME") << "Bink video error!" << std::endl;
    }
    bik.InitFramebuffer(vidFrame);
    vidSound = GameManager::CreateAudioSource(NULL, 0, false);

    return true;
}

int Game::Start() {
    /*if(!menu.Start()) {
        // exited out of intro or menu
        return EXIT_SUCCESS;
    }*/

    //AudioSource song = LoadSong("data/fx/music/appytimes.raw", true);
    //PlayAudio(song);

    while(Display::IsOpen()) {
        Display::Clear();
        Display::HandleEvents();
        Display::StartTimer();

        //Display::RenderTextureFullscreen(tgx);
        //Display::RenderTextureScale(tex1, 0.0, 0.0, 0.1, 0.1);
        //Display::RenderTextureScale(tex2, 0, 0, 30*8, 16*8);
        //Display::RenderTextureScale(tex2, 30*8, 0, 30*8, 16*8);
        //Display::RenderTextureScale(tex3, 0, 16*8, 30*8, 16*8);
        //Display::RenderTextureScale(tex4, 30*8, 16*8, 30*8, 16*8);

        bik.Decode(vidFrame, vidSound);
        Display::RenderTextureFullscreen(vidFrame);

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
