#ifndef GAMEOPTIONS_H_
#define GAMEOPTIONS_H_
#include <array>
#include <string>

struct Resolution {
  int width, height;
  bool operator==(const Resolution& other) const noexcept {
    return other.height == height && other.width == width;
  }
};

constexpr auto available_resolutions = std::array<Resolution, 10>  //
    {{{800, 600},
      {1024, 768},
      {1280, 720},
      {1280, 1024},
      {1366, 768},
      {1440, 900},
      {1600, 900},
      {1600, 1200},
      {1680, 1050},
      {1920, 1080}}};

struct GameOptions {
  std::string config_;
  std::string data_directory_path_;
  bool force_color_ = true;
  bool fullscreen_ = false;
  Resolution resolution_;
  int monitor_index_ = 0;
  bool noborder_ = false;
  bool nointro_ = false;
  bool nograb_ = false;
  bool nosound_ = false;
  bool nothread_ = false;
  bool nocache_ = false;
  bool debug_ = false;
};
#endif  // GAMEOPTIONS_H_
