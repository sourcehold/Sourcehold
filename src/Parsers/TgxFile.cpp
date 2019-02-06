#include <Parsers/TgxFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

TgxFile::TgxFile() : Parser(), Texture() {

}

TgxFile::~TgxFile() {

}

bool TgxFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path, std::fstream::in | std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to open Tgx file '" << path.native() << "'!" << std::endl;
        return false;
    }
    if(!Parser::GetData(&header, sizeof(TgxHeader))) {
        Logger::error("PARSERS") << "Unable to load Tgx file header from '" << path.native() << "'!" << std::endl;
        return false;
    }

    length = Parser::GetLength();

    /* Allocate image */
    Texture::AllocNew(header.width, header.height, SDL_PIXELFORMAT_RGBA8888);

    /* Read image data */
    ReadTokens();

    /* Copy image data to texture */
    Texture::UpdateTexture();

    return true;
}

void TgxFile::DumpInformation() {

}

int extractBits(int n, int k, int p) { 
    return (((1 << k) - 1) & (n >> (p - 1)));
} 

void TgxFile::ReadTokens() {
    uint32_t x = 0, y = 0;

    while(Parser::Ok()) {
        /* Read token byte */
        uint8_t b = Parser::GetByte();
        uint8_t len = (b & 0b11111) + 1;
        uint8_t flag = (b >> 5);

        if(flag == 0b100) { /* Newline */
            y++;
            x = 0; //x -= header.width;
        }else if(flag == 0b010) { /* Repeating pixel (they call _that_ compression lol) */
            uint16_t pixel = Parser::GetWord();
            uint8_t r,g,b;

            r = extractBits(pixel, 10, 5) * 8;
            g = extractBits(pixel, 5, 5) * 8;
            b = extractBits(pixel, 0, 5) * 8;

            /* Put the same pixel into buffer */
            for(uint8_t i = 0; i < len; ++i) {
                Texture::SetPixel(x, y, r, g, b);
                x++;
            }
        }else if(flag == 0b000) { /* Pixel stream */
            for(uint8_t i = 0; i < len; ++i) {
                uint16_t pixel = Parser::GetWord();
                uint8_t r,g,b;

            r = extractBits(pixel, 10, 5) * 8;
            g = extractBits(pixel, 5, 5) * 8;
            b = extractBits(pixel, 0, 5) * 8;

                Texture::SetPixel(x, y, r, g, b);

                x++;
            }
        }else if(flag == 0b001) {
            //std::cout << "Trans: " << (int)len << std::endl;
            x += len;
        }else {
            Logger::warning("PARSERS") << "Unknown token in tgx!" << std::endl;
        }
    }
}
