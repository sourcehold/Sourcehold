#include <MlbFile.h>

using namespace OpenSH::Parsers;
using namespace OpenSH::System;

MlbFile::MlbFile() : Parser() {

}

MlbFile::~MlbFile() {
    Clear();
}

// 1: E8 03 00 00 03 00 00 00 01 00 00 00 00 00 00 00 57 03 00 00 Länge = 1709 bytes, Offset = 0x014, Key = Streit, Modus = Belagerung, Index = 1
// 2:    00 00 00 05 00 00 00 01 00 00 00 01 00 00 00 4E 00 00 00 Länge = 0152 bytes, Offset = 0x6D4, Key = Flegel, Modus = 
// 3:    00 00 00 03 00 00 00 02 00 00 00 00 00 00 00 BE 01 00 00 Länge = 0890 bytes, Offset = 0x782, Key = Angers, Modus = 

void MlbFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path, std::ios::binary)) {
        Logger::LogToConsole("Unable to load Mlb file '" + path.native() + "' from data folder!", Logger::ERROR);
        return;
    }

    /* Parse header - 0x13 bytes */
    char header[0x13];
    Parser::GetData(header, 0x13);

    std::string s = Parser::GetUTF16();
    //std::cout << s << std::endl;
}

void MlbFile::Clear() {
    field.clear();
}
