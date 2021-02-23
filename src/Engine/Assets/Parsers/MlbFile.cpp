#include "MlbFile.h"

namespace Sourcehold {
namespace Parsers {
MlbFile::MlbFile() : Parser() {}

MlbFile::~MlbFile() { Clear(); }

bool MlbFile::LoadFromDisk(ghc::filesystem::path path) {
  using namespace System;
  if (!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
    Logger::error(PARSERS) << "Unable to load Mlb file " << path
                           << " from data folder!" << std::endl;
    return false;
  }

  num = Parser::GetWord();

  for (int i = 0; i < num; i++) {
    Parser::GetDWord();
    Parser::GetDWord();
    Parser::GetDWord();
    Parser::GetWord();
    auto len = Parser::GetDWord();

    std::wstring s = Parser::GetUTF16(len);
    if (!s.empty()) field.push_back(s);
  }

  Parser::Close();
  return true;
}

void MlbFile::Clear() { field.clear(); }
}  // namespace Parsers
}  // namespace Sourcehold
