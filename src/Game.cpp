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

    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << std::endl;

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
#include <string>
#include <vector>

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow) {
    /* Convert argument list */
    int w_argc = 0;
    LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
    if(w_argv) {
        std::vector<std::string> argv_buf;
        argv_buf.reserve(w_argc);

        for (int i = 0; i < w_argc; ++i) {
            int w_len = lstrlenW(w_argv[i]);
            int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
            std::string s;
            s.resize(len);
            WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, &s[0], len, NULL, NULL);
            argv_buf.push_back(s);
        }

        std::vector<char*> argv;
        argv.reserve(argv_buf.size());
        for (std::vector<std::string>::iterator i = argv_buf.begin(); i != argv_buf.end(); ++i)
            argv.push_back((char*)i->c_str());
        
        int code = main(argv.size(), &argv[0]);

        LocalFree(w_argv);
        return code;
    }

    int code = main(0, NULL);

    LocalFree(w_argv);
    return code;
}

#endif
