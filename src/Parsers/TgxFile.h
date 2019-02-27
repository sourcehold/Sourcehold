#pragma once

#include <cinttypes>
#include <bitset>

#include <Config.h>
#include <Parsers/Parser.h>
#include <System/Logger.h>
#include <Rendering/Texture.h>

using namespace Sourcehold::Rendering;

namespace Sourcehold
{
    namespace Parsers
    {
        class TgxFile : private Parser
        {
            public:
                struct TgxHeader {
                    uint16_t width;
                    uint16_t u0;
                    uint16_t height;
                    uint16_t u1;
                };

                TgxFile();
                ~TgxFile();

                bool LoadFromDisk(const std::string &path, Texture &target);

                static void ReadTgx(Parser &pa, Texture &tex, size_t size, uint16_t width, uint16_t height, uint16_t *pal);
                static void ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b);
            protected:
            private:
                TgxHeader header;
                uint32_t length = 0;
        };
    }
}
