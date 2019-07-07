#include <memory>
#include <cstdlib>
#include <string>
#include <vector>

#include <cxxopts.hpp>

#include <World.h>
#include <GameManager.h>

#include <GUI/Startup.h>
#include <Rendering/Font.h>
#include <Parsers/MlbFile.h>

#include <System/System.h>
#include <System/Logger.h>
#include <System/FileUtil.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Assets;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::GUI;

void Cleanup()
{
    UnloadFonts();
    ClearFileCache();
    DestroyManager();
}

int EnterLoadingScreen()
{
    std::shared_ptr<TgxFile> tgx_loading = GetTgx(GetDirectory() / "gfx/frontend_loading.tgx");

    /* Get the assets */
    std::vector<boost::filesystem::path> files = GetDirectoryRecursive(GetDirectory(), ".ani");
    if(files.empty()) {
        Logger::error("GAME") << "Here's a Nickel, kid. Go buy yourself a real Stronghold." << std::endl;
        return EXIT_FAILURE;
    }

    /* Preload some assets */
    uint32_t index = 0;

    /* Calculater the position */
    int px = (GetWidth() / 2) - (1024 / 2);
    int py = (GetHeight() / 2) - (768 / 2);

    ResetTarget();

    Resolution res = GetResolution();
    StrongholdEdition ed = GetEdition();

    while(Running() && index < files.size()-1) {
        ClearDisplay();

#if RENDER_LOADING_BORDER == 1
        if(ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
            RenderMenuBorder();
        }
#endif

        /* Load a file */
        boost::filesystem::path path = files.at(index);
        Cache(path);
        index++;

        /* Normalized loading progess */
        double progress = (double)index / (double)files.size();

        /* Render the background */
        Render(*tgx_loading, px, py);

        /* Render the loading bar */
        RenderRect(px+(1024/2)-(450/2), py+int(768.0/1.3), 450, 35, 0, 0, 0, 128, true);
        RenderRect(px+(1024/2)-(450/2), py+int(768.0/1.3), 450, 35, 0, 0, 0, 255, false);
        RenderRect(px+5+(1024/2)-(450/2), py+5+int(768.0/1.3), int(440.0*progress), 25, 0, 0, 0, 255, true);

        FlushDisplay();
        SyncDisplay();
    }

    return Running() ? EXIT_SUCCESS : EXIT_FAILURE;
}

int StartGame(GameOptions &opt)
{
    if(!InitManager(opt) || !LoadGameData()) {
        Logger::error("GAME") << "Error while initializing game!" << std::endl;
        return EXIT_FAILURE;
    }
    if(!LoadFonts()) {
        Logger::error("GAME") << "Error while loading fonts!" << std::endl;
        return EXIT_FAILURE;
    }
    if(!InitializeUtils()) {
        Logger::error("GAME") << "Error while initializing menu utils!" << std::endl;
        return EXIT_FAILURE;
    }

    /* Init the startup sequence */
    Startup start;
    start.PlayMusic();

    int ret = EnterLoadingScreen();
    if(ret != EXIT_SUCCESS) return ret;

    /* Start the intro sequence and the main menu */
    ret = start.Begin();
    if (ret != EXIT_SUCCESS) {
        Cleanup();
        return ret;
    }

    Cleanup();
    return EXIT_SUCCESS;
}

#undef main

/* Common entry point across all platforms */
int main(int argc, char **argv)
{
    /* Parse commandline */
    cxxopts::Options options("Sourcehold", "Open source engine implementation of Stronghold");
    options.add_options()
    ("h,help", "Print this info")
    ("config-file", "Path to custom config file", cxxopts::value<std::string>()->default_value("config.ini"))
    ("p,path", "Custom path to data folder", cxxopts::value<std::string>()->default_value("../data/"))
    ("d,debug", "Print debug info")
    ("color", "Force color output")
    ("f,fullscreen", "Run in fullscreen mode")
    ("r,resolution", "Resolution of the window", cxxopts::value<uint8_t>()->default_value("1"))
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

        opt.resolution = (Resolution)result["resolution"].as<uint8_t>();
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
    }
    catch(cxxopts::OptionException ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}

#if SOURCEHOLD_MINGW == 1 && 0

#include <windows.h>
#include <string>
#include <vector>

/* Windows specific entry point */
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
{
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
