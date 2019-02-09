#include <Game.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Sound;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;

Game::Game() : Display() {

}

Game::~Game() {
    
}

bool Game::Init(CmdLineOptions &opt) {
    Display::OpenWindowed("Sourcehold version " SOURCEHOLD_VERSION_STRING, 800, 600);

    Context ctx = Display::CreateContext();

    //MlbFile mlb;
    //mlb.LoadFromDisk(boost::filesystem::path("data/stronghold.mlb"));

    Gm1File gm1;
    if(!gm1.LoadFromDisk("data/gm/anim_armourer.gm1")) {
        Logger::error("GAME") << "Gm1 file loading error!" << std::endl;
    }

    tgx.SetContext(ctx);
    if(!tgx.LoadFromDisk("data/gfx/frontend_loading.tgx")) {
        Logger::error("GAME") << "Tgx file loading error!" << std::endl;
    }

    VolumeTxt txt;
    txt.LoadFromDisk("data/fx/volume.txt");
    int vol = txt.GetVolumeOf("fx\\music\\stainedglass1.raw");

    Sound::Init();
    Sound::PlayMusic("data/fx/music/Glory_01.raw", true);
    
    bik.Init(ctx);
    bik.LoadFromDisk("data/binks/intro.bik");

    return true;
}

int Game::MainLoop() {
    while(Display::IsOpen()) {
        Display::Clear();
        Display::HandleEvents();
        Display::StartTimer();

        // render everything
        //tgx.Render(0, 0);
        Display::RenderTextureFullscreen(tgx);
        //Display::RenderTextureXY(tgx, 10, 10);

        //Texture &frame = bik.RenderFrame();
        //Display::RenderTextureXY(frame, 0, 0);

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

    return game.MainLoop();
}

#if SOURCEHOLD_WINDOWS == 1
#include <windows.h>
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow) {
    return main(NULL, 0);
}
#endif
