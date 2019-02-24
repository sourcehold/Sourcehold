#include <Parsers/TgxFile.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

TgxFile::TgxFile() : Parser() {

}

TgxFile::~TgxFile() {

}

bool TgxFile::LoadFromDisk(std::string path, Texture &target) {
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
    target.AllocNew(header.width, header.height, SDL_PIXELFORMAT_RGBA8888);

    /* Calculate size */
    size_t size = length - sizeof(TgxHeader);

    /* Read image data */
    ReadTgx(*this, target, size, header.width, header.height, NULL);

    /* Copy image data to texture */
    target.UpdateTexture();

    return true;
}

void TgxFile::ReadTgx(Parser &pa, Texture &tex, size_t size, uint16_t width, uint16_t height, uint16_t *pal) {
    uint32_t x = 0, y = 0;
    size_t read = 0, offset = pa.Tell();

    while(pa.Ok() && read < size) {
        /* Read token byte */
        uint8_t b = pa.GetByte();
        uint8_t len = (b & 0b11111) + 1;
        uint8_t flag = b >> 5;

        switch(flag) {
            case 0b000: {
                for(uint8_t i = 0; i < len; ++i,++x) {
                    uint16_t pixelColor;
                    if(pal) {
                        uint8_t index = pa.GetByte();
                        pixelColor = pal[256 + index];
                    }else {
                        pixelColor = pa.GetWord();
                    }
                    uint8_t r,g,b;
                    ReadPixel(pixelColor, r, g, b);
                    tex.SetPixel(x, y, r, g, b, 0xFF);
                }
            }break;
            case 0b100: {
                y++;
                x = 0;
            }break;
            case 0b010: {
                uint16_t pixelColor;
                if(pal) {
                    uint8_t index = pa.GetByte();
                    pixelColor = pal[256 + index];
                }else {
                    pixelColor = pa.GetWord();
                }
                uint8_t r,g,b;
                ReadPixel(pixelColor, r, g, b);
                /* Put the same pixel into buffer */
                for(uint8_t i = 0; i < len; ++i,++x) {
                    tex.SetPixel(x, y, r, g, b, 0xFF);
                }
            }break;
            case 0b001: {
                x += len;
            }break;
            default: {
                Logger::error("PARSERS") << "Unknown token in gm1!" << std::endl;
            }break;
        }

        read = pa.Tell() - offset;
    }
}

void TgxFile::ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b) {
    r = ((pixel >> 10) & 0b11111) << 3;
    g = (((pixel >> 5) & 0b11111) | ((pixel >> 8) & 0b11)) << 3;
    b = (pixel & 0b11111) << 3;
}
