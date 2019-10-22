#include "Parsers/TexFile.h"
#include "System/Logger.h"

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

TexFile::TexFile() :
    Parser()
{
}

bool TexFile::LoadFromDisk(std::filesystem::path path)
{
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
        Logger::error(PARSERS) << "Unable to read tex file from " << path << std::endl;
        return false;
    }

    static uint32_t offsets[250];
    Parser::GetData(&offsets, sizeof(offsets));
    Parser::SeekG(0x3e8);

    for(uint32_t i = 0; i < 249; i++) {
        uint32_t fp = 0x3e8 + (offsets[i] << 1);
        uint32_t end = 0x3e8 + (offsets[i+1] << 1) - 9;
        Parser::SeekG(fp);

        while(Parser::Ok() && fp < end) {
            std::wstring str = Parser::GetUTF16();
            fp += str.size()*2 + 4;
            Parser::SeekG(fp);

            strings[(TextSection)i].push_back(str);
        }
    }

    Parser::Close();
    return true;
}

std::wstring &TexFile::GetString(TextSection sec, uint16_t index)
{
    if(index < strings.at(sec).size())
        return strings.at(sec)[index];

    Logger::warning(PARSERS) << "Trying to get string " << index << " from section " << sec << ", that only contain " << (strings.at(sec).size()-1) << " of these." << std::endl;
    return dummy_text;
}
