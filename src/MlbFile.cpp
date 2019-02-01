#include <MlbFile.h>

using namespace OpenSH::Parsers;
using namespace OpenSH::System;

MlbFile::MlbFile() : Parser() {

}

MlbFile::~MlbFile() {
    Clear();
}

void MlbFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path, std::ios::binary)) {
        Logger::LogToConsole("Unable to load Mlb file '" + path.native() + "' from data folder!", Logger::ERROR);
        return;
    }

    /* Parse header - 0x13 bytes */
    char header[0x13];
    Parser::GetData(header, 0x13);

    std::u16string s = Parser::GetUTF16();
}

void MlbFile::Clear() {
    field.clear();
}
