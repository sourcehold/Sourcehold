
#include <memory>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include "World.h"
#include "GameManager.h"
#include "Startup.h"

#include "Rendering/Font.h"
#include "Parsers/MlbFile.h"

#include "System/System.h"
#include "System/Logger.h"
#include "System/FileUtil.h"

#include "GUI/NarrScreen.h"
#include "GUI/MainMenu.h"

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Assets;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::GUI;

void Cleanup()
{
    SaveConfig();
    UnloadFonts();
    ClearFileCache();
    DestroyManager();
}

int MainLoop(UIState state) {
    switch(state) {
    case MAIN_MENU: {
        MainMenu menu;
        state = menu.EnterMenu();

        MainLoop(state);
    } break;
    case MILITARY_CAMPAIGN_MISSION: {
        int index = 0; // todo

        NarrScreen narr(index + 1);
        narr.Begin();

        // TODO
        Song music(GetDirectory() / "fx/music/the maidenA.raw", true);
        music.Play();

        World world;
        world.LoadFromDisk(GetDirectory() / "maps/mission1.map");
        state = world.Play();

        music.Stop();

        MainLoop(state);
    } break;
    case EXIT_GAME: break;
    default: break;
    }

    Cleanup();
    return EXIT_SUCCESS;
}

int EnterLoadingScreen()
{
    std::shared_ptr<TgxFile> tgx_loading = GetTgx("gfx/frontend_loading.tgx");

    /* Get the assets */
    std::vector<boost::filesystem::path> files = GetDirectoryRecursive(GetDirectory(), ".ani");
    if(files.empty()) {
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
        RenderRect(Rect<int>{ px+(1024/2)-(450/2), py+int(768.0/1.3), 450, 35 }, 0, 0, 0, 128, true);
        RenderRect(Rect<int>{ px+(1024/2)-(450/2), py+int(768.0/1.3), 450, 35 }, 0, 0, 0, 255, false);
        RenderRect(Rect<int>{ px+5+(1024/2)-(450/2), py+5+int(768.0/1.3), int(440.0*progress), 25 }, 0, 0, 0, 255, true);

        FlushDisplay();
        SyncDisplay();
    }

    return Running() ? EXIT_SUCCESS : EXIT_FAILURE;
}

int StartGame(GameOptions &opt)
{
    if(!InitManager(opt) || !LoadGameData()) {
        Logger::error(GAME) << "Error while initializing game!" << std::endl;
        ErrorMessageBox(
            "Here's a Nickel, kid. Go buy yourself a real Stronghold",
            std::string("Please make sure the data directory contains all necessary files.\n") +
            + "Current path: " +
            GetDirectory().string()
        );
        return EXIT_FAILURE;
    }
    if(!LoadFonts()) {
        Logger::error(GAME) << "Error while loading fonts!" << std::endl;
        return EXIT_FAILURE;
    }

    Startup start;

    int ret = EnterLoadingScreen();
    if (ret != EXIT_SUCCESS) return ret;

    start.PlayMusic();
    UIState state = start.Begin();

    return MainLoop(state);
}

#undef main

/* Common entry point across all platforms */
int main(int argc, char **argv)
{
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

    fs::path full_path(fs::initial_path<fs::path>());
    full_path = fs::system_complete(fs::path(argv[0])).parent_path();

    // Parse commandline / config file //
    try {
        GameOptions opt;

        // Command line only //
        po::options_description generic("Generic options");
        generic.add_options()
            ("help,h", po::bool_switch()->default_value(false), "Print this info")
            ("version,v", "Print version string");

        // Command line and config file //
        po::options_description config("Settings");
        config.add_options()
            ("path,p", po::value<std::string>(&opt.dataDir)->default_value("../data/"), "Custom path to data folder")
            ("debug", po::bool_switch(&opt.debug)->default_value(false), "Print debug info")
            ("color,c", po::bool_switch()->default_value(false), "Force color output")
            ("fullscreen,f", po::bool_switch(&opt.fullscreen)->default_value(false), "Run in fullscreen mode")
            ("resolution,r", po::value<int>()->default_value(1), "Resolution of the window")
            ("disp,d", po::value<uint16_t>(&opt.ndisp)->default_value(0), "Index of the monitor to be used")
            ("noborder", po::bool_switch(&opt.noborder)->default_value(false), "Remove window border")
            ("nograb", po::bool_switch(&opt.nograb)->default_value(false), "Don't grab the mouse")
            ("nosound", po::bool_switch(&opt.nosound)->default_value(false), "Disable sound entirely")
            ("nothread", po::bool_switch(&opt.nothread)->default_value(false), "Disable threading")
            ("nocache", po::bool_switch(&opt.nocache)->default_value(false), "Disable asset caching");

        po::options_description config_file_options;
        config_file_options.add(config);

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        po::variables_map result;
        po::store(po::parse_command_line(argc, argv, cmdline_options), result);

        if (boost::filesystem::exists(full_path / "settings.ini")) {
            std::ifstream ifs(fs::path(full_path / "settings.ini").string().c_str());
            po::store(po::parse_config_file(ifs, config_file_options, true), result);
            ifs.close();
        }
        else {
            Logger::warning(GAME) << "There appears to be no settings.ini in your installation path!" << std::endl;
        }

        po::notify(result);

        if (result["help"].as<bool>()) {
            std::cout << config << std::endl;
            return EXIT_SUCCESS;
        }

        if (result.count("color") > 0) opt.color = result["color"].as<bool>();
        else opt.color = -1;

        opt.resolution = (Resolution)result["resolution"].as<int>();

        boost::erase_all(opt.dataDir, "\""); // can sometimes appear when there's extra "" in the .ini

        return StartGame(opt);
    }
    catch (po::error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
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
