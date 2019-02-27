#include <Parsers/MlbFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

MlbFile::MlbFile() : Parser() {

}

MlbFile::~MlbFile() {
    Clear();
}

bool MlbFile::LoadFromDisk(const std::string &path) {
    if(!Parser::Open(path, std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to load Mlb file '" << path << "' from data folder!" << std::endl;
        return false;
    }

    /* Parse header - 0x13 bytes */
    char header[0x13];
    if(!Parser::GetData(header, 0x13)) {
        Logger::error("PARSERS")  << "Unable to parse Mlb header '" << path << "'!" << std::endl;
        return false;
    }

    std::string s = Parser::GetUTF16();

    Parser::Close();

    return true;
}

void MlbFile::Clear() {
    field.clear();
}
