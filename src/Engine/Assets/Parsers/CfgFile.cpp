#include "CfgFile.h"

namespace Sourcehold {
namespace Parsers {
void CfgFile::SetDefaultValues() {
  username = L"Lord Julian";
  speed = 30;
  bubbleHelp = 0;
  resolution = 3;
  soundEnabled = 1;
  musicVolume = 50;
  sfxVolume = 50;
  unlockedMilitary = 1;
  unlockedEconomic = 1;
  speechVolume = 50;
  mousePointer = 1;
}

bool CfgFile::LoadFromDisk(ghc::filesystem::path path) {
  using namespace System;
  if (!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
    Logger::error(PARSERS) << "Unable to read cfg " << path << std::endl;
    return false;
  }

  Parser::SeekG(0x46);
  username = Parser::GetUTF16();
  Parser::SeekG(0x246);
  speed = Parser::GetByte();
  Parser::SeekG(0x24A);
  bubbleHelp = Parser::GetByte();
  Parser::SeekG(0x24E);
  resolution = Parser::GetByte();
  Parser::SeekG(0x25A);
  soundEnabled = Parser::GetByte();
  Parser::SeekG(0x25E);
  musicVolume = Parser::GetByte();
  Parser::SeekG(0x262);
  sfxVolume = Parser::GetByte();
  Parser::SeekG(0x26A);
  unlockedMilitary = Parser::GetByte();
  Parser::SeekG(0x26E);
  unlockedEconomic = Parser::GetByte();
  Parser::SeekG(0x266);
  speechVolume = Parser::GetByte();
  Parser::SeekG(0x292);
  mousePointer = Parser::GetByte();

  Parser::Close();
  return true;
}

bool CfgFile::WriteToDisk(ghc::filesystem::path path) {
  using namespace System;
  if (!Parser::Open(path.string(), std::ios::binary | std::ofstream::out)) {
    Logger::error(PARSERS) << "Unable to write cfg " << path << std::endl;
    return false;
  }

  /* TODO */
  Parser::SeekP(0x46);
  Parser::WriteUTF16(username);
  Parser::SeekP(0x246);
  Parser::WriteByte(speed);
  Parser::SeekP(0x24A);
  Parser::WriteByte(bubbleHelp);
  Parser::SeekP(0x24E);
  Parser::WriteByte(resolution);
  Parser::SeekP(0x25A);
  Parser::WriteByte(soundEnabled);
  Parser::SeekP(0x25E);
  Parser::WriteByte(musicVolume);
  Parser::SeekP(0x262);
  Parser::WriteByte(sfxVolume);
  Parser::SeekP(0x26A);
  Parser::WriteByte(unlockedMilitary);
  Parser::SeekP(0x26E);
  Parser::WriteByte(unlockedEconomic);
  Parser::SeekP(0x266);
  Parser::WriteByte(speechVolume);
  Parser::SeekP(0x292);
  Parser::WriteByte(mousePointer);
  Parser::SeekP(0x329);
  Parser::WriteByte(0x00);

  Parser::Close();
  return true;
}
}  // namespace Parsers
}  // namespace Sourcehold
