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

    /* Calculate size */
    size_t size = length - sizeof(TgxHeader);

    /* Read image data */
    ReadTgx(this, this, size);

    /* Copy image data to texture */
    Texture::UpdateTexture();

    return true;
}

void TgxFile::DumpInformation() {

}

void TgxFile::ReadTgx(Parser *pa, Texture *tex, size_t size) {
    uint32_t x = 0, y = 0;
    size_t read = 0, offset = pa->Tell();

    while(pa->Ok() && read < size) {
        /* Read token byte */
        uint8_t b = pa->GetByte();
        uint8_t len = (b & 0b11111) + 1;
        uint8_t flag = (b >> 5);

        if(flag == 0b100) { /* Newline */
            y++;
            x = 0;
        }else if(flag == 0b010) { /* Repeating pixel */
            uint8_t r,g,b;
            ReadPixel(pa->GetWord(), &r, &g, &b);
            /* Put the same pixel into buffer */
            for(uint8_t i = 0; i < len; ++i,++x) {
                tex->SetPixel(x, y, r, g, b);
            }
        }else if(flag == 0b000) { /* Pixel stream */
            for(uint8_t i = 0; i < len; ++i,++x) {
                uint8_t r,g,b;
                ReadPixel(pa->GetWord(), &r, &g, &b);
                tex->SetPixel(x, y, r, g, b);
            }
        }else if(flag == 0b001) { /* Transparent pixel stream */
            x += len;
        }else if(flag == 0b111) { /* Seems to be end-of-stream */
            //break;
        }else {
            std::bitset<8> tk(b);
            std::bitset<3> fg(flag);
            //Logger::warning("PARSERS") << "Unknown token in tgx: 0b" << tk << ", extracted length: " << (int)len << ", flag: 0b" << fg << std::endl;
        }

        read = pa->Tell() - offset;
    }
}

void TgxFile::ReadPixel(uint16_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = ((pixel >> 10) & 0b11111) << 3;
    *g = (((pixel >> 5) & 0b11111) | ((pixel >> 8) & 0b11)) << 3;
    *b = (pixel & 0b11111) << 3;
}
