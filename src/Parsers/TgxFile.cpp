#include <Parsers/TgxFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

TgxFile::TgxFile() : Parser(), Texture() {

}

TgxFile::~TgxFile() {

}

bool TgxFile::LoadFromDisk(std::string path) {
    if(!Parser::Open(path, std::fstream::in | std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to open Tgx file '" << path << "'!" << std::endl;
        return false;
    }
    if(!Parser::GetData(&header, sizeof(TgxHeader))) {
        Logger::error("PARSERS") << "Unable to load Tgx file header from '" << path << "'!" << std::endl;
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

void TgxFile::ReadTokens() {
    uint32_t x = 0, y = 0;

    while(Parser::Ok()) {
        /* Read token byte */
        uint8_t b = Parser::GetByte();
        uint8_t len = (b & 0b11111) + 1;
        uint8_t flag = (b >> 5);

        if(flag == 0b100) { /* Newline */
            y++;
            x = 0;
        }else if(flag == 0b010) { /* Repeating pixel (they call _that_ compression lol) */
            uint8_t r,g,b;
            ReadPixel(Parser::GetWord(), &r, &g, &b);

            /* Put the same pixel into buffer */
            for(uint8_t i = 0; i < len; ++i,++x) {
                Texture::SetPixel(x, y, r, g, b);
            }
        }else if(flag == 0b000) { /* Pixel stream */
            for(uint8_t i = 0; i < len; ++i,++x) {
                uint8_t r,g,b;
                ReadPixel(Parser::GetWord(), &r, &g, &b);
                Texture::SetPixel(x, y, r, g, b);
            }
        }else if(flag == 0b001) { /* Transparent pixel stream */
            x += len;
        }else if(flag == 0b111) { /* Seems to be end-of-stream */
            break;
        }else {
            std::bitset<8> tk(b);
            std::bitset<3> fg(flag);
            Logger::warning("PARSERS") << "Unknown token in tgx: 0b" << tk << ", extracted length: " << (int)len << ", flag: 0b" << fg << std::endl;
        }
    }
}

void TgxFile::ReadPixel(uint16_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = 8 * ((pixel >> 0x02) & 0b11111);
    *g = 8 * ((pixel >> 0x0d) << 2 | (pixel & 0b11));
    *b = 8 * ((pixel >> 0x08) & 0b11111);
}
