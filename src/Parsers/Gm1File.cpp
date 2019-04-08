#include <algorithm>
#include <vector>
#include <thread>
#include <atomic>
#include <climits>

#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>
#include <System/Logger.h>

using namespace Sourcehold::Parsers;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

struct Gm1File::ImageHeader {
    /* Image dimensions */
    uint16_t width;
    uint16_t height;
    /* Image offsets */
    uint16_t offsetX;
    uint16_t offsetY;
    /* Image part */
    uint8_t part;
    uint8_t parts;
    uint16_t tileOffsetY;
    /* Left/Right/Center */
    enum Direction : uint8_t {
        DIR_NONE =  0,
        DIR_DOWN =  1,
        DIR_RIGHT = 2,
        DIR_LEFT =  3,
    } direction;
    /* Horizontal offset */
    uint8_t horizOffset;
    /* Width of building part */
    uint8_t partWidth;
    /* Color */
    uint8_t color;
};

struct Gm1File::Gm1Entry {
    ImageHeader header;
    uint32_t size;
    uint32_t offset;
    uint16_t collection;
    uint16_t offX, offY;
};

Gm1File::Gm1File(std::shared_ptr<Renderer> rend) : Parser()
{
    this->renderer = rend;

    textureAtlas = std::make_shared<TextureAtlas>(rend);
    tileset = std::make_shared<Tileset>(rend);
}

Gm1File::Gm1File(std::shared_ptr<Renderer> rend, const std::string &path) : Parser()
{
    this->renderer = rend;

    textureAtlas = std::make_shared<TextureAtlas>(rend);
    tileset = std::make_shared<Tileset>(rend);

    this->LoadFromDisk(path, false);
}

Gm1File::~Gm1File() {

}

bool Gm1File::LoadFromDisk(const std::string &path, bool threaded) {
    this->path = path;

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

    /* Reserve size in vectors to fit all entries */
    entries.resize(header.num);

    /* Read Gm1 palette */
    ReadPalette();

    uint32_t n;

    /* Read offsets */
    for(n = 0; n < header.num; n++) {
        Parser::GetData(&(entries.begin() + n)->offset, sizeof(uint32_t));
    }

    /* Read sizes */
    for(n = 0; n < header.num; n++) {
        Parser::GetData(&(entries.begin() + n)->size, sizeof(uint32_t));
    }

    for(n = 0; n < header.num; n++) {
        /* Read image header */
        Parser::GetData(&(entries.begin() + n)->header, sizeof(ImageHeader));
    }

     /* Get offset of data start */
    offData = Parser::Tell();

    /* Read compressed images into buffer */
    uint32_t buflen = Parser::GetLength() - offData;
    imgdata = new char[buflen];
    Parser::GetData(imgdata, buflen);

    /* Close file */
    Parser::Close();

    /**
     * For tilesets, find out when a new set of
     * tile/image combinations begins. A subset of the
     * tileset will correspond to a single image in
     * the texture atlas (i.e. building + ground).
     */
    numCollections = 0;
    for(n = 0; n < header.num; n++) {
        if(entries[n].header.part == 0 && header.type == Gm1Header::TYPE_TILE) {
            numCollections++;
        }
    }

    if(header.type == Gm1Header::TYPE_TILE) {
        /* Allocate collections */
        std::vector<std::pair<uint32_t, uint32_t>> entryDims(numCollections);
        uint32_t currentEntry = 0;
        for(n = 0; n < numCollections; n++) {
            int32_t top = INT_MAX, left = INT_MAX, bottom = 0, right = 0;
            uint32_t cnt = uint32_t(entries[currentEntry].header.parts);

            /* Go through every entry and find the image bounds */
            for(int i = currentEntry; i < currentEntry + cnt; i++) {
                int32_t x = int32_t(entries[i].header.offsetX);
                int32_t y = int32_t(entries[i].header.offsetY);
                int32_t w = int32_t(entries[i].header.width);
                int32_t h = int32_t(entries[i].header.height);

                left = std::min(left, x);
                top = std::min(top, y);
                right = std::max(right, x+w);
                bottom = std::max(bottom, y+h);
            }

            /* Hand the width/height to the texture atlas  */
            entryDims[n] = std::make_pair<uint32_t, uint32_t>(uint32_t(right-left), uint32_t(bottom-top));

            /* Calculate the offset relative to the texture atlas rectangle for every entry */
            for(uint32_t d = currentEntry + cnt; currentEntry < d; currentEntry++) {
                entries[currentEntry].collection = n;
                entries[currentEntry].offX = uint16_t(entries[currentEntry].header.offsetX + uint16_t(entries[n].header.horizOffset)) - left;
                entries[currentEntry].offY = uint16_t(entries[currentEntry].header.offsetY) - top;
            }
        }
        /* One collection -> one texture */
        textureAtlas->Allocate(entryDims);
        textureAtlas->LockTexture();
        tileset->Allocate(header.num);
        tileset->LockTexture();
        for(n = 0; n < entries.size(); n++) {
            GetImage(n);
        }
        tileset->UnlockTexture();
        textureAtlas->UnlockTexture();
    }else {
        /* Allocate images */
        std::vector<std::pair<uint32_t, uint32_t>> entryDims(header.num);
        for(n = 0; n < header.num; n++) {
            std::pair<uint32_t, uint32_t> dim(entries[n].header.width, entries[n].header.height);
            entryDims[n] = dim;
        }
        textureAtlas->Allocate(entryDims);
        textureAtlas->LockTexture();
        /* One entry -> one texture */
        for(n = 0; n < entries.size(); n++) {
            GetImage(n);
        }
        textureAtlas->UnlockTexture();
    }

    delete [] imgdata;
    return true;
}

void Gm1File::DumpInformation() {
    Logger::message("PARSERS") << "Gm1 file:\nNum: " << header.num << "\nType: " << header.type << "\nLen: " << header.len << std::endl;
}

void Gm1File::Free() {
    entries.clear();
    textureAtlas->Clear();
    tileset->Clear();
}

bool Gm1File::GetImage(uint32_t index) {
    /* Seek to position */
    char *position = imgdata + entries[index].offset;

    switch(header.type) {
        case Gm1Header::TYPE_INTERFACE: case Gm1Header::TYPE_FONT: case Gm1Header::TYPE_CONSTSIZE: {
            SDL_Rect part = textureAtlas->Get(index);
            TgxFile::ReadTgx(*textureAtlas, position, entries[index].size, part.x, part.y, nullptr);
        }break;
        case Gm1Header::TYPE_ANIMATION: {
            SDL_Rect part = textureAtlas->Get(index);
            TgxFile::ReadTgx(*textureAtlas, position, entries[index].size, part.x, part.y, palette);
        }break;
        case Gm1Header::TYPE_TILE: {
            /* Read tile */
            SDL_Rect tile = tileset->GetTile(index);
            SDL_Rect part = textureAtlas->Get(entries[index].collection);

            /* Extract pixel data */
            const static uint8_t lines[16] = {
                2, 6, 10, 14, 18, 22, 26, 30,
                30, 26, 22, 18, 14, 10, 6, 2
            };

            /* Read every line */
            for(uint8_t l = 0; l < 16; l++) {
                /* Read every pixel in a line */
                for(uint8_t i = 0; i < lines[l]; i++) {
                    uint16_t pixel = *reinterpret_cast<uint16_t*>(position);
                    position += 2;

                    /* Read RGB */
                    uint8_t r, g, b;
                    TgxFile::ReadPixel(pixel, r, g, b);

                    /* Add to tileset texture */
                    tileset->SetPixel(
                        tile.x + (15 - lines[l] / 2 + i),
                        tile.y + l,
                        r, g, b, 0xFF
                    );
                }
            }

            TgxFile::ReadTgx(*textureAtlas, position, entries[index].size - 512, part.x + entries[index].offX, part.y + entries[index].offY, nullptr);
        }break;
        case Gm1Header::TYPE_MISC1: case Gm1Header::TYPE_MISC2: {
            SDL_Rect part = textureAtlas->Get(index);

            for(uint32_t x = 0; x < entries[index].header.width; x++) {
                for(uint32_t y = 0; y < entries[index].header.height; y++) {
                    uint16_t pixel = *reinterpret_cast<uint16_t*>(position);
                    position += 2;

                    /* Read RGB */
                    uint8_t r, g, b;
                    TgxFile::ReadPixel(pixel, r, g, b);

                    /* Add to texture */
                    textureAtlas->SetPixel(x+part.x, y+part.y, r, g, b, 0xFF);
                }
            }
        }break;
        default: {
            Logger::error("PARSERS") << "Unknown filetype stored in Gm1 '" << path << "': " << header.type << std::endl;
            return false;
        }
    }

    return true;
}

void Gm1File::ReadPalette() {
    if(!Parser::GetData(palette, sizeof(palette))) {
        Logger::error("PARSERS") << "Unable to read palette from Gm1!" << std::endl;
        Parser::Close();
    }
}
