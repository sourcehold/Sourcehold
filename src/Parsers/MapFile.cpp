#include <cstring>
#include <cmath>

#include "Parsers/MapFile.h"
#include "Parsers/TgxFile.h"

#include "System/Logger.h"

extern "C" {
#include <blast.h>
}

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

struct MapSectionHeader {
    uint32_t uncompressedLen;
    uint32_t compressedLen;
    uint32_t crc32;
};

struct MapFile::MapSec {
    uint8_t* data;
    uint32_t num;
};

MapFile::MapFile()
{
}

MapFile::MapFile(ghc::filesystem::path path)
{
    LoadFromDisk(path);
}

MapFile::~MapFile()
{
}

bool MapFile::LoadFromDisk(ghc::filesystem::path path)
{
    if (!Parser::Open(path.string(), std::ifstream::in | std::ios::binary)) {
        Logger::error(PARSERS) << "Unable to open map file '" << path.string() << "'!" << std::endl;
        return false;
    }

    // magic number
    if (0xFFFFFFFF != Parser::GetDWord()) {
        Logger::error(PARSERS) << "Magic number of map file '" << path.string() <<"' is wrong!" << std::endl;
        return false;
    }

    // unknown int
    Parser::GetDWord();

    ParsePreview();

    Parser::Close();
    return true;
}

MapFile::MapSec MapFile::BlastSection()
{
    MapSec sec = { NULL, 0 };

    // TODO: better value
    const static uint32_t MAX_COMPRESSED_LEN = 1024 * 32;

    MapSectionHeader header;
    Parser::GetData(&header, sizeof(header));

    if (header.compressedLen > MAX_COMPRESSED_LEN) {
        Logger::error(PARSERS) << "Map section exceeds the maximum size of " << MAX_COMPRESSED_LEN << " bytes!" << std::endl;
        return sec;
    }

    // "blast" using zlib, TODO: less hacks
    unsigned left = 0;
    uint8_t* outbuf = new uint8_t[header.uncompressedLen];

    struct InData {
        Parser* parser;
        uint32_t compLen; // length of compressed chunk
        uint32_t readPos; // position in buffer
    } indat;

    struct OutData {
        uint8_t* buf;
        uint32_t uncompLen;
        uint32_t writePos;
    } outdat;

    indat.parser = this;
    indat.compLen = header.compressedLen;
    indat.readPos = 0;

    outdat.buf = outbuf;
    outdat.uncompLen = header.uncompressedLen;
    outdat.writePos = 0;

    int ret = blast(
        [](void *how, unsigned char **buf) -> unsigned {
            static unsigned char hold[16384];
            InData* data = static_cast<InData*>(how);

            *buf = hold;
            uint32_t num = 16384;
            if (data->readPos + num > data->compLen) {
                // shrink input to fit compressed data
                num -= data->compLen - data->readPos;
            }

            data->parser->GetData(hold, num);
            data->readPos += num;

            return num;
        },
        &indat,
        [](void *how, unsigned char *buf, unsigned len) -> int {
            OutData* data = static_cast<OutData*>(how);

            uint32_t num = len;
            if (data->writePos + len > data->uncompLen) {
                // buffer overflow, write as much as possible
                num -= data->uncompLen - (data->writePos + len);
            }

            std::memcpy(data->buf+data->writePos, buf, num);
            data->writePos += num;

            return num != len;
        },
        &outdat,
        &left,
        NULL
    );

    switch (ret) {
    case  2: Logger::error(PARSERS) << "PKWARE blast: Out of input!" << std::endl; break;
    case  1: Logger::error(PARSERS) << "PKWARE blast: Output error!" << std::endl; break;
    case -1: Logger::error(PARSERS) << "PKWARE blast: Literal flag not zero or one!" << std::endl; break;
    case -2: Logger::error(PARSERS) << "PKWARE blast: Dictionary size not in rane 4-6!" << std::endl; break;
    case -3: Logger::error(PARSERS) << "PKWARE blast: Distance is too far back!" << std::endl; break;
    default: break;
    }

    uint32_t crc = ComputeCRC32(outbuf, outdat.writePos);
    if(crc != header.crc32) {
        Logger::warning(PARSERS) << "CRC32 checksum mismatch in map section!" << std::endl;
    }

    sec.data = outbuf;
    sec.num = outdat.writePos;
    return sec;
}

void MapFile::ParsePreview()
{
    // Thanks to https://github.com/gynt //
    MapSec prev = BlastSection();

    const static uint32_t PALSIZE = 512;
    uint32_t dim = std::sqrt(prev.num - PALSIZE);

    // copy pixels into temporary surface
    Surface surf;
    surf.AllocNew(dim, dim);
    surf.LockSurface();

    Uint8 r, g, b, a;
    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            uint8_t index = prev.data[PALSIZE+x+y*dim];
            TgxFile::ReadPixel(reinterpret_cast<uint16_t*>(prev.data)[index], r, g, b, a);
            surf.SetPixel(x, y, r, g, b, a);
        }
    }

    surf.UnlockSurface();
    preview.AllocFromSurface(surf);
    delete[] prev.data;
}

uint32_t MapFile::ComputeCRC32(const void *data, size_t size)
{
    //boost::crc_32_type res;
    //res.process_bytes(data, size);
    //return res.checksum();
    return 0;
}

