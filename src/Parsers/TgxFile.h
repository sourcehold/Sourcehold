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
#include <Rendering/Surface.h>

using namespace Sourcehold::Rendering;

namespace Sourcehold
{
    namespace Parsers
    {
        using Rendering::Texture;

        /*
         * File containing a single image, provides parsing functions
         * for GM1 files too
         */
        class TgxFile : private Parser, public Texture
        {
            std::shared_ptr<Renderer> renderer;
        public:
                TgxFile(std::shared_ptr<Renderer> rend);
                TgxFile(std::shared_ptr<Renderer> rend, const std::string &path);
                ~TgxFile();

                bool LoadFromDisk(const std::string &path);
                void Unload();

                static void ReadTgx(Surface &tex, char *buf, size_t size, uint16_t offX, uint16_t offY, uint16_t *pal);
                static void ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b);
            protected:
                struct TgxHeader;
        };
    }
}
