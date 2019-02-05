#include <Parsers/TgxFile.h>

using namespace OpenSH::Parsers;
using namespace OpenSH::System;

TgxFile::TgxFile() : Parser(), Texture() {

}

TgxFile::~TgxFile() {

}

bool TgxFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path, std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to open Tgx file '" << path.native() << "'!" << std::endl;
        return false;
    }
    if(!Parser::GetData(&header, sizeof(TgxHeader))) {
        Logger::error("PARSERS") << "Unable to load Tgx file header from '" << path.native() << "'!" << std::endl;
        return false;
    }

    /* Allocate image */
    Texture::AllocNew(header.width, header.height, SDL_PIXELFORMAT_RGB332);

    /* Read image data */
    ReadToken();

    return true;
}

void TgxFile::DumpInformation() {

}

void TgxFile::ReadToken() {
    static uint16_t x = 0, y = 0;
    bool stream=0,newline=0,repeat=0,trans=0;

    if(!Parser::Ok()) return;

    /* Read flags */
    uint8_t b = Parser::GetByte();
    uint8_t len = (b ^ 0b11100000) + 1;
    b >>= 5;
    if(b & 0b00000000) { stream = true; }
    if(b & 0b00000100) { newline = true; len = 0; }
    if(b & 0b00000010) { repeat = true; }
    if(b & 0b00000001) { trans = true; }

    /* Read data */
    if(newline) {
        y++;
        x = 0;
    }else {
        if(repeat) {
            uint16_t pixel = Parser::GetWord();
            uint8_t r,g,b;
            b = (pixel >> 8) ^ 0b11100000;
            g = (pixel >> 11) | (pixel ^ 0b1111111111111100);
            r = (pixel >> 2) ^ 0b1111111111100000;
            for(uint8_t i = 0; i < len; ++i) {
            }
            x += len;
        }else {
            for(uint8_t i = 0; i < len; ++i) {
                uint8_t r,g,b;
                uint16_t pixel = Parser::GetWord();
                b = (pixel >> 8) ^ 0b11100000;
                g = (pixel >> 11) | (pixel ^ 0b1111111111111100);
                r = (pixel >> 2) ^ 0b1111111111100000;
                x++;
            }
        }
    }

    ReadToken();
}
