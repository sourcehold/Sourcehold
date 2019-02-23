#include <Parsers/Gm1File.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Gm1File::Gm1File() :
    Parser()
{

}

Gm1File::~Gm1File() {

}

bool Gm1File::LoadFromDisk(std::string path) {
    if(!Parser::Open(path, std::fstream::in | std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to open Gm1 file '" << path << "'!" << std::endl;
        return false;
    }

    if(!Parser::GetData(&header, sizeof(Gm1Header))) {
        Logger::error("PARSERS") << "Unable to load Gm1 file header from '" << path << "'!" << std::endl;
        Parser::Close();
        return false;
    }

    /* Boundary check */
    if(header.num > max_num) {
        Logger::error("PARSERS") << "Gm1 file header from '" << path << "' contains too many images!" << std::endl;
        Parser::Close();
        return false;
    }

    /* Reserve size in vector to fit all entries */
    entries.resize(header.num);

    /* Read Gm1 palette */
    ReadPalette();

    /* Read offsets */
    for(uint32_t n = 0; n < header.num; n++) {
        Parser::GetData(&entries[n].offset, sizeof(uint32_t));
    }

    /* Read sizes */
    for(uint32_t n = 0; n < header.num; n++) {
        Parser::GetData(&entries[n].size, sizeof(uint32_t));
    }

    /* Read image headers */
    for(uint32_t n = 0; n < header.num; n++) {
        Parser::GetData(&entries[n].header, sizeof(ImageHeader));
    }

    /* Get offset of data start */
    uint32_t offData = Parser::Tell();

    /* Parse the entries */
    switch(header.type) {
        case Gm1Header::TYPE_INTERFACE: case Gm1Header::TYPE_FONT: case Gm1Header::TYPE_CONSTSIZE: case Gm1Header::TYPE_ANIMATION: {
            for(uint32_t n = 0; n < header.num; n++) {
                /* Seek to position */
                Parser::Seek(entries[n].offset + offData);

                /* Error handling */
                if(!Parser::Ok()) {
                    Logger::error("PARSERS") << "Error while parsing image " << n << " from Gm1 '" << path << "'!" << std::endl;
                    Parser::Close();
                    return false;
                }

                /* Read texture */
                /*Texture tex(ctx);
                tex.AllocNew(entries[n].header.width, entries[n].header.height, SDL_PIXELFORMAT_RGBA8888);

                TgxFile::ReadTgx(this, &tex, entries[n].size, entries[n].header.width, entries[n].header.height);
                tex.UpdateTexture();

                entries[n].image = tex;*/
            }
        }break;
        case Gm1Header::TYPE_TILE: {
            for(uint32_t n = 0; n < header.num; n++) {
                /* Seek to position */
                Parser::Seek(entries[n].offset + offData);
                
                /* Error handling */
                if(!Parser::Ok()) {
                    Logger::error("PARSERS") << "Error while parsing tile " << n << " from Gm1 '" << path << "'!" << std::endl;
                    Parser::Close();
                    return false;
                }
                
                /* Create texture */
                /*Texture tex;
                tex.SetContext(ctx);
                tex.AllocNew(30, 16, SDL_PIXELFORMAT_RGBA8888);*/
                
                /* Extract pixel data */
                const uint8_t lines[16] = {
                    2, 6, 10, 14, 18, 22, 26, 30,
                    30, 26, 22, 18, 14, 10, 6, 2
                };

                for(uint8_t l = 0; l < 16; l++) {
                    /* Read every pixel in a line */
                    for(uint8_t i = 0; i < lines[l]; i++) {
                        uint16_t pixel = Parser::GetWord();
                
                        /* Read RGB */
                        uint8_t r, g, b;
                        TgxFile::ReadPixel(pixel, r, g, b);
                
                        /* Add to texture */
                        //tex.SetPixel(i, l, r, g, b, 0xFF);
                    }
                }
                /* Write pixels to texture */
                //tex.UpdateTexture();
                
                /* Add texture to list */
                //entries[n].image = tex;
            }
        }break;
        case Gm1Header::TYPE_MISC1: case Gm1Header::TYPE_MISC2: {
            Logger::warning("PARSERS") << "Type not handled at the moment!" << std::endl;
        }break;
        default: {
            Logger::error("PARSERS") << "Unknown filetype stored in Gm1 '" << path << "': " << header.type << std::endl;
            Parser::Close();
            return false;
        }
    }

    Parser::Close();
    return true;
}

void Gm1File::DumpInformation() {
    Logger::message("PARSERS") << "Gm1 file:\nNum: " << header.num << "\nType: " << header.type << "\nLen: " << header.len << std::endl;
}

std::vector<Gm1File::Gm1Entry>& Gm1File::GetEntries() {
    return entries;
}

Gm1File::Gm1Entry &Gm1File::GetEntry(uint32_t index) {
    return entries[index];
}

void Gm1File::ReadPalette() {
    uint8_t palette[5120];
    if(!Parser::GetData(palette, sizeof(palette))) {
        Logger::error("PARSERS") << "Unable to read palette from Gm1!" << std::endl;
    }
}
