#include <Game.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;

Game::Game() : GameManager(), menu(*this) {

}

Game::~Game() {
    
}

bool Game::Init(CmdLineOptions &opt) {
    if(opt.fullscreen) {
        GameManager::Fullscreen();
    }

    return true;
}

int Game::Start() {
    const std::string p = "data/gfx/frontend_combat2.tgx", p1 = "data/gm/body_trebutchet.gm1";

    Texture tex = Renderer::CreateTexture();
    TgxFile tgx;
    tgx.LoadFromDisk(p, tex);

    Gm1File gm1;
    TextureAtlas atlas = Renderer::CreateTextureAtlas();
    gm1.LoadFromDisk(p1, atlas);

    enum Trebutchet : uint16_t {
        ANIMATION_1,
        ANIMATION_2,
        ANIMATION_3
    };

    AnimationHandler::AddSlot(Trebutchet::ANIMATION_1, atlas, { 0, 10 });

    while(GameManager::Running()) {
        GameManager::Clear();
        GameManager::HandleEvents();
        GameManager::StartTimer();

        GameManager::RenderTextureFullscreen(tex);
        GameManager::RenderTextureScale(atlas.Get(AnimationHandler::GetFrame(Trebutchet::ANIMATION_1)), 0.0, 0.0, 0.5, 0.5);

        GameManager::Flush();
        GameManager::EndTimer();
        GameManager::Update();
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    Game game;
    CmdLineOptions opt = ParseCmdLine(argc, argv);

    if(!opt.valid) return EXIT_FAILURE;
    if(opt.info) return EXIT_SUCCESS;
    
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
