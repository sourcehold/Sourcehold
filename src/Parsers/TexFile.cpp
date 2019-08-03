#include <Parsers/TexFile.h>
#include <System/Logger.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

TexFile::TexFile() :
    Parser()
{
}

bool TexFile::LoadFromDisk(boost::filesystem::path path)
{
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
        Logger::error(PARSERS) << "Unable to read tex file from " << path << std::endl;
        return false;
    }

    static uint32_t offsets[250];
    Parser::GetData(&offsets, sizeof(offsets));
    Parser::SeekG(0x3e8);

    for(uint32_t i = 0; i < 250; i++) {
        uint32_t fp = 0x3e8 + (offsets[i] << 1);
        Parser::SeekG(fp);

        std::wstring sectionDesc = Parser::GetUTF16();
        // TODO
    }

    Parser::Close();
    return true;
}
