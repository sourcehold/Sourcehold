#include "Commandline.h"
#include <cxxopts.hpp>
static cxxopts::Options createCLIConfig(GameOptions& options) {
  using namespace cxxopts;
  Options config("Sourcehold", "Open-source Stronghold");

  config.add_options()                                        //
      ("h,help", "Print this info")                           //
      ("v,version", "Print version string")                   //
      ("resolutions", "List available resolutions and exit")  //
      ("p,path", "Custom path to data folder",
       value<std::string>(options.data_directory_path_)               //
           ->default_value(options.data_directory_path_))             //
      ("debug", "Print debug info",                                   //
       value<bool>(options.debug_)                                    //
           ->default_value(options.debug_ ? "true" : "false"))        //
      ("c,color", "Force color output",                               //
       value<bool>(options.force_color_)                              //
           ->default_value(options.force_color_ ? "true" : "false"))  //
      ("f,fullscreen", "Run in fullscreen mode",
       value<bool>(options.fullscreen_)                              //
           ->default_value(options.fullscreen_ ? "true" : "false"))  //
      ("resolution", "Resolution of the window",
       value<std::vector<int>>())  //
      ("d,disp", "Index of the monitor to be used",
       value<int>(options.monitor_index_)
           ->default_value(std::to_string(options.monitor_index_)))  //
      ("s,skip", "Skip directly to the main menu",                   //
       value<bool>(options.nointro_)                                 //
           ->default_value(options.nointro_ ? "true" : "false"))     //
      ("noborder", "Remove window border",                           //
       value<bool>(options.noborder_)                                //
           ->default_value(options.noborder_ ? "true" : "false"))    //
      ("nograb", "Don't grab the mouse",                             //
       value<bool>(options.nograb_)                                  //
           ->default_value(options.nograb_ ? "true" : "false"))      //
      ("nosound", "Disable sound entirely",                          //
       value<bool>(options.nosound_)                                 //
           ->default_value(options.nosound_ ? "true" : "false"))     //
      ("nothread", "Disable threading",                              //
       value<bool>(options.nothread_)                                //
           ->default_value(options.nothread_ ? "true" : "false"))    //
      ("nocache", "Disable asset caching",                           //
       value<bool>(options.nocache_)                                 //
           ->default_value(options.nocache_ ? "true" : "false"));

  return config;
}

// This will exit the application if these flags are set:
//  --help
//  --resolutions
static void handleExitOptions(cxxopts::ParseResult& parse_result,
                              cxxopts::Options& config) {
  if (parse_result["help"].as<bool>()) {
    std::cout << config.help() << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  if (parse_result["resolutions"].as<bool>()) {
    for (const auto& resolution : available_resolutions) {
      std::cout << resolution.width << "," << resolution.height << std::endl;
    }
    std::exit(EXIT_SUCCESS);
  }
}

static void setResolution(cxxopts::ParseResult& parse_result,
                          GameOptions& options) {
  if (parse_result.count("resolution")) {
    const auto resolution = parse_result["resolution"].as<std::vector<int>>();
    if (resolution.size() == 2) {
      options.resolution_.width = resolution[0];
      options.resolution_.height = resolution[1];
    } else {
      throw std::runtime_error(
          "Resolution input argument malformed. Try: {WIDHT, HEIGHT}");
    }
  } else {
    options.resolution_.width = 800;
    options.resolution_.height = 600;
  }
}

GameOptions getGameOptions(int argc, char** argv) {
  using namespace cxxopts;
  try {
    GameOptions options;

    auto config = createCLIConfig(options);
    auto parse_result = config.parse(argc, argv);

    handleExitOptions(parse_result, config);
    setResolution(parse_result, options);

    return options;

  } catch (OptionException& e) {
    throw;
  }
}
