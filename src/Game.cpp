#include <memory>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <cxxopts.hpp>

#include <MainMenu.h>
#include <World.h>
#include <GameManager.h>

#include <System/System.h>
#include <System/Logger.h>

#include <Assets/Assets.h>

#include <Rendering/Font.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Assets;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;

int StartGame(GameOptions &opt) {
    auto gameManager = std::make_shared<GameManager>(opt);

    /* Get the assets */
    gameManager->SetDirectory(opt.dataDir);
    std::vector<std::string> files = GetDirectoryRecursive(opt.dataDir);
    if(files.empty()) {
        Logger::error("GAME") << "The 'data' directory is empty; did you copy all the necessary files?" << std::endl;
        return EXIT_FAILURE;
    }

    if(!LoadFonts(gameManager)) return EXIT_FAILURE;

    std::shared_ptr<TgxFile> tgx_loading = gameManager->GetTgx(gameManager->GetDirectory() + "gfx/frontend_loading.tgx").lock();

    /* Init the menu */
    MainMenu menu(gameManager);
    menu.PlayMusic();

    uint32_t index = 0;
    while(gameManager->Running() && index < files.size()-1) {
        gameManager->Clear();

        /* Load a file */
        std::string path = files.at(index);
        gameManager->Cache(path);
        index++;

        /* Normalized loading progess */
        double progress = (double)index / (double)files.size();

        /* Render the background */
        gameManager->Render(*tgx_loading);

        /* Render the loading bar */
        gameManager->Render(0.3, 0.75, 0.4, 0.04, 0, 0, 0, 128, true);
        gameManager->Render(0.3, 0.75, 0.4, 0.04, 0, 0, 0, 255, false);
        gameManager->Render(0.305, 0.755, 0.39 * progress, 0.03, 0, 0, 0, 255, true);

        gameManager->Flush();
    }

    if(gameManager->Running()) {
        /* Start the intro sequence and the main menu */
        int ret = menu.Startup();
        if(ret != EXIT_SUCCESS) return ret;

        /* ------ Alpha testing ------ */

        AudioSource aud("data/fx/music/sadtimesa.raw", true);
        aud.Play();

        World world(gameManager);
        world.Play();
        /* ------ Alpha testing ------ */
    }

    gameManager->ClearFileCache();
    UnloadFonts();
    return EXIT_SUCCESS;
}

/* Common entry point across all platforms */
int main(int argc, char **argv) {
    /* Parse commandline */
    cxxopts::Options options("Sourcehold", "Open source engine implementation of Stronghold");
    options.add_options()
        ("h,help", "Print this info")
        ("config-file", "Path to custom config file", cxxopts::value<std::string>()->default_value("config.ini"))
        ("p,path", "Custom path to data folder", cxxopts::value<std::string>()->default_value("./data/"))
        ("d,debug", "Print debug info")
        ("color", "Force color output")
        ("f,fullscreen", "Run in fullscreen mode")
        ("width", "Width of the window", cxxopts::value<uint16_t>()->default_value("800"))
        ("height", "Height of the window", cxxopts::value<uint16_t>()->default_value("600"))
        ("disp", "Index of the monitor to be used", cxxopts::value<uint16_t>()->default_value("0"))
        ("noborder", "Remove window border")
        ("nograb", "Don't grab the mouse")
        ("nosound", "Disable sound entirely")
        ("nothread", "Disable threading");

    try {
        GameOptions opt;
        auto result = options.parse(argc, argv);

        if(result["help"].as<bool>()) {
            std::cout << options.help(options.groups()) << std::endl;
            return EXIT_SUCCESS;
        }

        opt.config = result["config-file"].as<std::string>();
        opt.dataDir = result["path"].as<std::string>();

        if(result["debug"].as<bool>()) {
            opt.debug = true;
        }

        if(result.count("color") > 0) opt.color = result["color"].as<bool>();
        else opt.color = -1;

        if(result["fullscreen"].as<bool>()) {
            opt.fullscreen = true;
        }

        opt.width = result["width"].as<uint16_t>();
        opt.height = result["height"].as<uint16_t>();
        opt.ndisp = result["disp"].as<uint16_t>();

        if(result["noborder"].as<bool>()) {
            opt.noborder = true;
        }
        if(result["nograb"].as<bool>()) {
            opt.nograb = true;
        }
        if(result["nosound"].as<bool>()) {
            opt.nosound = true;
        }
        if(result["nothread"].as<bool>()) {
            opt.nothread = true;
        }

        return StartGame(opt);
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

/* Windows specific entry point */
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
