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
        class TgxFile : private Parser, public Texture
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

                bool LoadFromDisk(std::string path);
                void DumpInformation();
            protected:
                void ReadTokens();
                void ReadPixel(uint16_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);
            private:
                TgxHeader header;
                uint32_t length = 0;
        };
    }
}
