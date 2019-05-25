#include <Parsers/CfgFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

CfgFile::CfgFile() : Parser() {
}

void CfgFile::SetDefaultValues() {
	username = L"Lord Stronghold";
	speed = 30;
	bubbleHelp = 0;
	resolution = 3;
	soundEnabled = 1;
	musicVolume = 50;
	sfxVolume = 50;
	speechVolume = 50;
	mousePointer = 1;
}

bool CfgFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
    	Logger::error("PARSERS") << "Unable to read cfg " << path << std::endl;
        return false;
    }

    Parser::Seek(0x46);
    username = Parser::GetUTF16();
    Parser::Seek(0x246);
    speed = Parser::GetByte();
    Parser::Seek(0x24A);
    bubbleHelp = Parser::GetByte();
    Parser::Seek(0x24E);
    resolution = Parser::GetByte();
    Parser::Seek(0x25A);
    soundEnabled = Parser::GetByte();
    Parser::Seek(0x25E);
    musicVolume = Parser::GetByte();
    Parser::Seek(0x262);
    sfxVolume = Parser::GetByte();
    Parser::Seek(0x266);
    speechVolume = Parser::GetByte();
    Parser::Seek(0x292);
    mousePointer = Parser::GetByte();

    Parser::Close();
    return true;
}

bool CfgFile::WriteToDisk(boost::filesystem::path path) {
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::out)) {
    	Logger::error("PARSERS") << "Unable to write cfg " << path << std::endl;
        return false;
    }

    Parser::Close();
	return true;
}

