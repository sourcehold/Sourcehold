#include <Game.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;

Game::Game(GameOptions &opt) :
    GameManager(opt)
{
}

Game::~Game() {
    
}

int Game::Start() {
    TgxFile tgx(renderer);
    tgx.LoadFromDisk("data/gfx/frontend_combat2.tgx");

    Gm1File gm1(renderer);
    gm1.LoadFromDisk("data/gm/body_trebutchet.gm1");

    enum Trebutchet : uint16_t {
        ANIMATION_1,
        ANIMATION_2,
        ANIMATION_3
    };

    while(GameManager::Running()) {
        renderer->Clear();
        renderer->HandleEvents();
        renderer->StartTimer();

        renderer->RenderTextureFullscreen(tgx);

        renderer->Flush();
        renderer->EndTimer();
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    /* Parse commandline */
    cxxopts::Options options("Sourcehold", "Open source engine implementation of Stronghold");
    options.add_options()
    ("h,help", "Print this info")
    ("config-file", "Path to custom config file", cxxopts::value<std::string>()->default_value("config.ini"))
    ("d,debug", "Print debug info")
    ("noborder", "Remove window border")
    ("f,fullscreen", "Run in fullscreen mode")
    ("width", "Width of the window", cxxopts::value<uint16_t>()->default_value("800"))
    ("height", "Height of the window", cxxopts::value<uint16_t>()->default_value("600"));

    try {
        GameOptions opt;
        auto result = options.parse(argc, argv);

        if(result["help"].as<bool>()) {
            std::cout << options.help(options.groups()) << std::endl;
            return EXIT_SUCCESS;
        }
        opt.config = result["config-file"].as<std::string>();
        if(result["debug"].as<bool>()) {
            opt.debug = true;
        }
        if(result["noborder"].as<bool>()) {
            opt.noborder = true;
        }
        if(result["fullscreen"].as<bool>()) {
            opt.fullscreen = true;
        }
        opt.width = result["width"].as<uint16_t>();
        opt.height = result["height"].as<uint16_t>();

        Game game(opt);
        return game.Start();
    }catch(cxxopts::OptionException ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
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
