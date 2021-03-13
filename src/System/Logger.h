#pragma once

#include <iostream>
#include <cinttypes>
#include <fstream>
#include <sstream>

#include "System/Config.h"

namespace Sourcehold {
namespace System {
enum Subsystem : uint8_t {
  AUDIO,
  EVENTS,
  GUI,
  PARSERS,
  RENDERING,
  GAME,
  SOURCEHOLD,
  ECS
};

/*
 * Logger class, redirects logging messages to a file,
 * stdout or any ostream provider (TODO!)
 */
class Logger {
 public:
  Logger();
  ~Logger();

  enum class LogType { ERROR, WARNING, MESSAGE, SUCCESS };

  static std::ostream &log(LogType type, const std::string &subsystem);
  static std::ostream &error(Subsystem subsystem = SOURCEHOLD);
  static std::ostream &warning(Subsystem subsystem = SOURCEHOLD);
  static std::ostream &message(Subsystem subsystem = SOURCEHOLD);
  static std::ostream &success(Subsystem subsystem = SOURCEHOLD);

  static void SetColorOutput(bool a);
  static bool GetColorOutput();

 private:
  static std::string SubsystemName(Subsystem sys);
};
}  // namespace System
}  // namespace Sourcehold
