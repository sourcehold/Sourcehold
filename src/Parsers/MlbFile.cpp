#include <Parsers/MlbFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

#pragma pack(push, 1)
struct MlbFile::SectionHeader {
    uint32_t f1;
    uint32_t f2;
    /* Unknown (Either 1 or 0, flag?) */
    uint32_t f3;
    /* Length of the following string (in characters) */
    uint32_t length;
};
#pragma pack(pop)

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
        Logger::error("PARSERS")  << "Unable to load Mlb file " << path << " from data folder!" << std::endl;
        return false;
    }

    num = Parser::GetDWord();

    for(int i = 0; i < num; i++) {
        SectionHeader header;
        if(!Parser::GetData(&header, 16)) {
            Logger::error("PARSERS")  << "Unable to parse Mlb subheader " << i << " from " << "'" << path << "'!" << std::endl;
            Parser::Close();
            return true;
        }

        std::wstring s = Parser::GetUTF16();
        if(!s.empty()) field.push_back(s);
    }

    Parser::Close();
    return true;
}

void MlbFile::Clear()
{
    field.clear();
}
