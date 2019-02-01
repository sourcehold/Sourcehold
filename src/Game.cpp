#include <Game.h>

using namespace OpenSH::Game;
using namespace OpenSH::Sound;
using namespace OpenSH::System;
using namespace OpenSH::Parsers;

Game::Game() : Display() {

}

Game::~Game() {
    
}

bool Game::Init(int argc, char **argv) {
    Display::OpenWindowed("OpenSH version " OPENSH_VERSION_STRING, 800, 600);

    boost::filesystem::path dataPath { "data" };
    data_folder = dataPath.native();

    MlbFile mlb;
    mlb.LoadFromDisk(boost::filesystem::path("data/stronghold.mlb"));

    OpenSH::Sound::Sound sound;
    sound.Init();

    return true;
}

int Game::MainLoop() {
    while(1) {
        Display::Clear();

        // render everything

        Display::Update();
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
