#include "Parsers/MlbFile.h"

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

MlbFile::MlbFile() : Parser()
{

}

MlbFile::~MlbFile()
{
    Clear();
}

bool MlbFile::LoadFromDisk(boost::filesystem::path path)
{
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
        Logger::error(PARSERS)  << "Unable to load Mlb file " << path << " from data folder!" << std::endl;
        return false;
    }

    num = Parser::GetWord();

    for(int i = 0; i < num; i++) {
        Parser::GetDWord();
        Parser::GetDWord();
        Parser::GetDWord();
        Parser::GetWord();
        uint16_t len = Parser::GetDWord();

        std::wstring s = Parser::GetUTF16(len);
        if(!s.empty()) field.push_back(s);
    }

    Parser::Close();
    return true;
}

void MlbFile::Clear()
{
    field.clear();
}
