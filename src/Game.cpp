#include <memory>
#include <cstdlib>
#include <string>
#include <vector>
#include <regex>

#include "System/Commandline.h"

#include "World.h"
#include "GameManager.h"
#include "Startup.h"

#include "Parsers/MlbFile.h"

#include "System/System.h"
#include "System/Logger.h"
#include "System/FileUtil.h"

#include "ECS/Manager.h"

#include "GUI/NarrScreen.h"
#include "GUI/MainMenu.h"

#ifdef SOURCEHOLD_IOS
  #include "Common/SHPathUtils.h"
#endif

using namespace Sourcehold;
using namespace Game;
using namespace Audio;
using namespace System;
using namespace Assets;
using namespace Parsers;
using namespace Rendering;
using namespace GUI;

int MainLoop(UIState state) {
  switch (state) {
    case MAIN_MENU: {
      state = std::make_unique<MainMenu>()->EnterMenu();

      MainLoop(state);
    } break;
    case EXIT_GAME:
      break;
    default:
      break;
  }

  ExitGame();
  return EXIT_SUCCESS;
}

int EnterLoadingScreen() {
  std::shared_ptr<TgxFile> tgx_loading = GetTgx("gfx/frontend_loading.tgx");

  /* Get the assets */
  std::vector<ghc::filesystem::path> files =
      GetDirectoryRecursive(GetDirectory(), ".ani");
  if (files.empty()) {
    return EXIT_FAILURE;
  }

  /* Preload some assets */
  uint32_t index = 0;

  /* Calculate the position */
  int px = (GetWidth() / 2) - (1024 / 2);
  int py = (GetHeight() / 2) - (768 / 2);

  ResetTarget();

  Resolution res = GetResolution();
  StrongholdEdition ed = GetEdition();

  while (Running() && index < files.size() - 1) {
    ClearDisplay();

#if RENDER_LOADING_BORDER == 1
    if (ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
      RenderMenuBorder();
    }
#endif

    /* Load a file */
    ghc::filesystem::path path = files.at(index);
    Cache(path);
    index++;

    /* Normalized loading progess */
    double progress = (double)index / (double)files.size();

    /* Render the background */
    Render(*tgx_loading, px, py);

    /* Render the loading bar */
    RenderRect(
        Rect<int>{px + (1024 / 2) - (450 / 2), py + int(768.0 / 1.3), 450, 35},
        0, 0, 0, 128, true);
    RenderRect(
        Rect<int>{px + (1024 / 2) - (450 / 2), py + int(768.0 / 1.3), 450, 35},
        0, 0, 0, 255, false);
    RenderRect(Rect<int>{px + 5 + (1024 / 2) - (450 / 2),
                         py + 5 + int(768.0 / 1.3), int(440.0 * progress), 25},
               0, 0, 0, 255, true);

    FlushDisplay();

    SDL_Delay(1);
  }

  // TODO - Maybe add next loading bar?
  ECS::Manager::GetInstance();

  return Running() ? EXIT_SUCCESS : EXIT_FAILURE;
}

static const int resolutions[][2] = {{800, 600},   {1024, 768},  {1280, 720},
                                     {1280, 1024}, {1360, 768},  {1366, 768},
                                     {1440, 900},  {1600, 900},  {1600, 1200},
                                     {1680, 1050}, {1920, 1080}, {-1, -1}};

int StartGame(GameOptions &opt) {
  // Init logger //
#if SOURCEHOLD_UNIX == 1 && !defined(SOURCEHOLD_ANDROID)
  Logger::SetColorOutput(true);
#else
  Logger::SetColorOutput(false);
#endif

  if (opt.color >= 0)
    Logger::SetColorOutput(opt.color == 1);

  Resolution res;
  // Convert resolution //
  bool found = false;
  for (int i = 0; i < (sizeof(resolutions) / sizeof(int)) / 2; i++) {
    int w = resolutions[i][0];
    int h = resolutions[i][1];

    if (w == opt.width && h == opt.height) {
      res = static_cast<Resolution>(i);
      found = true;
      break;
    }
  }

  if (!found) {
    /**
     * If the selected resolution is not supported by the original game,
     * some graphical elements will have to be scaled or positioned differently.
     */
    res = RESOLUTION_DYNAMIC;
    Logger::warning(GAME) << "You selected an unsupported resolution. This may "
                             "lead to ugly scaling!"
                          << std::endl;
  }

  // Init game //
  Logger::message(GAME) << "Starting version " SOURCEHOLD_VERSION_STRING
                           " (" SOURCEHOLD_BUILD ")"
                        << std::endl;

  if (!InitManager(opt, res) || !LoadGameData()) {
    ErrorMessageBox("Something went wrong...",
                    std::string("Please make sure the data directory contains "
                                "all necessary files.\n") +
                        +"Current path: " + GetDirectory().string());
    return EXIT_FAILURE;
  }

  Logger::message(GAME) << "Done" << std::endl;

  auto start = std::make_unique<Startup>();

  int ret = EnterLoadingScreen();
  if (ret != EXIT_SUCCESS)
    return ret;

  UIState state = MAIN_MENU;
  if (!opt.skip) {
    start->PlayMusic();
    state = start->Begin();
  }

  return MainLoop(state);
}

// Do not #undef main on iOS, because in other case it will be not possible to
// build project
#ifndef SOURCEHOLD_IOS
  #undef main
#endif  // SOURCEHOLD_IOS

/* Common entry point across all platforms */
int main(int argc, char **argv) {
  namespace po = cxxopts;
  namespace fs = ghc::filesystem;

  // Parse commandline //
  try {
    GameOptions opt;

    po::Options config("Sourcehold", "Open-source Stronghold");

    config.add_options()("h,help", "Print this info")("v,version",
                                                      "Print version string")(
        "resolutions", "List available resolutions and exit")(
        "p,path", "Custom path to data folder",
        po::value<std::string>()->default_value("../data/"))(
        "debug", "Print debug info", po::value<bool>(opt.debug))(
        "c,color", "Force color output")("f,fullscreen",
                                         "Run in fullscreen mode",
                                         po::value<bool>(opt.fullscreen))(
        "r,resolution", "Resolution of the window",
        po::value<std::string>()->default_value("1280x720"))(
        "d,disp", "Index of the monitor to be used",
        po::value<uint16_t>()->default_value("0"))(
        "s,skip", "Skip directly to the main menu", po::value<bool>(opt.skip))(
        "noborder", "Remove window border", po::value<bool>(opt.noborder))(
        "nograb", "Don't grab the mouse", po::value<bool>(opt.nograb))(
        "nosound", "Disable sound entirely", po::value<bool>(opt.nosound))(
        "nothread", "Disable threading", po::value<bool>(opt.nothread))(
        "nocache", "Disable asset caching", po::value<bool>(opt.nocache));

    auto result = config.parse(argc, argv);
    if (result["help"].as<bool>()) {
      std::cout << config.help() << std::endl;
      return EXIT_SUCCESS;
    }

    if (result["resolutions"].as<bool>()) {
      for (int i = 0; i < (sizeof(resolutions) / sizeof(int)) / 2; i++) {
        std::cout << resolutions[i][0] << "x" << resolutions[i][1] << std::endl;
      }
      return EXIT_SUCCESS;
    }

    if (result.count("color") > 0)
      opt.color = result["color"].as<bool>();
    else
      opt.color = -1;

    opt.ndisp = result["disp"].as<uint16_t>();
#ifdef SOURCEHOLD_IOS
    char resourcesPath[512];

    SHQueryMainBundleResourcesDirectoryPath(resourcesPath, 512);
    opt.dataDir = std::string(resourcesPath).append("/data");
#else
    opt.dataDir = result["path"].as<std::string>();
#endif  // SOURCEHOLD_IOS

    std::regex regex("(\\d+)x(\\d+)");
    std::smatch match;

#if defined(SOURCEHOLD_ANDROID) || defined(SOURCEHOLD_IOS)
    // Width and height arguments will be ignored on Android and iOS //
    opt.width = opt.height = -1;
#else
    const std::string str = result["resolution"].as<std::string>();
    if (std::regex_search(str.begin(), str.end(), match, regex)) {
      opt.width = std::stoi(match[1]);
      opt.height = std::stoi(match[2]);
    }
    else {
      // Fallback to 800x600 if the resolution is ill-formed
      opt.width = 800;
      opt.height = 600;
    }
#endif  // defined(SOURCEHOLD_ANDROID) || defined(SOURCEHOLD_IOS)

    return StartGame(opt);
  }
  catch (po::OptionException &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}

#if SOURCEHOLD_MINGW == 1 && 0

  #include <windows.h>
  #include <string>
  #include <vector>

/* Windows specific entry point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument,
                   int iShow) {
  /* Convert argument list */
  int w_argc = 0;
  LPWSTR *w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
  if (w_argv) {
    std::vector<std::string> argv_buf;
    argv_buf.reserve(w_argc);

    for (int i = 0; i < w_argc; ++i) {
      int w_len = lstrlenW(w_argv[i]);
      int len =
          WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
      std::string s;
      s.resize(len);
      WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, &s[0], len, NULL, NULL);
      argv_buf.push_back(s);
    }

    std::vector<char *> argv;
    argv.reserve(argv_buf.size());
    for (std::vector<std::string>::iterator i = argv_buf.begin();
         i != argv_buf.end(); ++i)
      argv.push_back((char *)i->c_str());

    int code = main(argv.size(), &argv[0]);

    LocalFree(w_argv);
    return code;
  }

  int code = main(0, NULL);

  LocalFree(w_argv);
  return code;
}

#endif
