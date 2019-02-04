#include <Game.h>

using namespace OpenSH::Game;
using namespace OpenSH::Sound;
using namespace OpenSH::System;
using namespace OpenSH::Parsers;
using namespace OpenSH::Rendering;

Game::Game() : Display() {

}

Game::~Game() {
    
}

bool Game::Init(int argc, char **argv) {
    Display::OpenWindowed("OpenSH version " OPENSH_VERSION_STRING, 800, 600);

    Context ctx = Display::CreateContext();

    boost::filesystem::path dataPath { "data" };
    data_folder = dataPath.native();

    MlbFile mlb;
    mlb.LoadFromDisk(boost::filesystem::path("data/stronghold.mlb"));

    Gm1File gm1;
    if(!gm1.LoadFromDisk(boost::filesystem::path("data/gm/anim_armourer.gm1"))) {
        Logger::error("GAME") << "Gm1 file loading error!" << std::endl;
    }

    tgx.SetContext(ctx);
    if(!tgx.LoadFromDisk(boost::filesystem::path("data/gfx/ap_civil10_first.tgx"))) {
        Logger::error("GAME") << "Tgx file loading error!" << std::endl;
    }

    Sound::Init();
    Sound::PlayMusic(boost::filesystem::path("data/fx/music/sadtimesa.raw"), true);
    
    bik.Init(ctx);
    bik.LoadFromDisk(boost::filesystem::path("data/binks/intro.bik"));

    return true;
}

int Game::MainLoop() {
    while(1) {
        Display::Clear();
        Display::HandleEvents();
        Display::StartTimer();

        // render everything
        tgx.Render(0, 0);

        SDL_Texture *t = bik.RenderFrame();

        Display::Flush();
        Display::EndTimer();
        if(!Display::IsOpen()) break;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    Game game;
    if(!game.Init(argc, argv)) {
        return EXIT_FAILURE;
    }

    return game.MainLoop();
}
