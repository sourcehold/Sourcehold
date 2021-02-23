#ifndef MLB_FILE_H_
#define MLB_FILE_H_

#include <fstream>
#include <vector>

#include "Logger.h"
#include "Parser.h"
#include "AssetsDef.h"

#include "FileSystem.h"

namespace Sourcehold {
namespace Parsers {
/*
 * The file containing all of the localized
 * mission descriptions.
 */
class MlbFile : private Parser {
  /* Encoded in UTF-16LE */
  std::vector<std::wstring> field;
  uint16_t num = 0;

 public:
  MlbFile();
  ~MlbFile();

  bool LoadFromDisk(ghc::filesystem::path path);
  void Clear();

  inline std::wstring& GetString(Assets::MissionDescription index) {
    return field.at(index);
  }

 protected:
  struct SectionHeader;
};
}  // namespace Parsers
}  // namespace Sourcehold
#endif  // MLB_FILE_H_
