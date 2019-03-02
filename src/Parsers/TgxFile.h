#pragma once

#include <cinttypes>
#include <algorithm>
#include <bitset>
#include <memory>

#include <Config.h>

#include <Parsers/Parser.h>

#include <System/Logger.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

using namespace Sourcehold::Rendering;

namespace Sourcehold
{
    namespace Parsers
    {
        using Rendering::Texture;

        class TgxFile : private Parser, public Texture
        {
            public:
                struct TgxHeader {
                    uint16_t width;
                    uint16_t u0;
                    uint16_t height;
                    uint16_t u1;
                };

                TgxFile(std::shared_ptr<Renderer> rend);
                ~TgxFile();

                bool LoadFromDisk(const std::string &path);

                static void ReadTgx(Texture &tex, char *buf, size_t size, uint16_t width, uint16_t height, uint16_t *pal);
                static void ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b);
            protected:
            private:
                TgxHeader header;
                uint32_t length = 0;
        };
    }
}
