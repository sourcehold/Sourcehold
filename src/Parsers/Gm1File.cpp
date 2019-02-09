#include <Parsers/Gm1File.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

Gm1File::Gm1File() : Parser() {

}

Gm1File::~Gm1File() {

}

bool Gm1File::LoadFromDisk(std::string path) {
    if(!Parser::Open(path, std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to open Gm1 file '" << path << "'!" << std::endl;
        return false;
    }
    if(!Parser::GetData(&header, sizeof(Gm1Header))) {
        Logger::error("PARSERS") << "Unable to load Gm1 file header from '" << path << "'!" << std::endl;
        return false;
    }
    if(header.num > max_num || header.type > 0x07 || header.type == 0x06) {
        Logger::warning("PARSERS") << "Gm1 file '" << path << "' may be damaged!" << std::endl;
    }
    if(header.len != Parser::GetLength() - sizeof(Gm1Header)) {
        Logger::warning("PARSERS") << "Gm1 file header from '" << path << "' contains wrong data length!" << std::endl;
    }

    for(uint32_t i = 0; i < header.num-1; ++i) {
        ImageHeader img;
        if(!Parser::GetData(&img, sizeof(ImageHeader))) {
            return false;
        }
    }

    Parser::Close();

    return true;
}

void Gm1File::DumpInformation() {
    Logger::message("PARSERS") << "Gm1 file:\nNum: " << header.num << "\nType: " << header.type << "\nLen: " << header.len << std::endl;
}
