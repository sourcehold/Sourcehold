#pragma once

#include "FileSystem.h"
#include <cinttypes>
#include <map>
#include <vector>

#include "Parser.h"

#include "AssetsDef.h"

namespace Sourcehold {
namespace Parsers {
using namespace Assets;

class TexFile : private Parser {
 public:
  TexFile();
  ~TexFile(){};

  bool LoadFromDisk(ghc::filesystem::path path);

  std::wstring &GetString(TextSection sec, uint16_t index);

 protected:
  std::map<TextSection, std::vector<std::wstring>> strings;
  std::wstring dummy_text = L"Text not found";
};
}  // namespace Parsers
}  // namespace Sourcehold
