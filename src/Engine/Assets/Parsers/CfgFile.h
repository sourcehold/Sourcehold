#ifndef CFG_FILE_H_
#define CFG_FILE_H_

#include <string>
#include <cinttypes>

#include "FileSystem.h"

#include "Logger.h"
#include "Parser.h"

namespace Sourcehold {
namespace Parsers {
class CfgFile : private Parser {
 public:
  CfgFile() = default;
  ~CfgFile() = default;

  void SetDefaultValues();

  bool LoadFromDisk(ghc::filesystem::path path);
  bool WriteToDisk(ghc::filesystem::path path);

  std::wstring username;
  uint8_t speed;
  uint8_t bubbleHelp;
  uint8_t resolution;
  uint8_t soundEnabled;
  uint8_t musicVolume;
  uint8_t sfxVolume;
  uint8_t unlockedMilitary;
  uint8_t unlockedEconomic;
  uint8_t speechVolume;
  uint8_t mousePointer;
};
}  // namespace Parsers
}  // namespace Sourcehold
#endif  // CFG_FILE_H_
