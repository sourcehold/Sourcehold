#pragma once

#include <cinttypes>
#include <algorithm>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>

#include <System/Logger.h>

#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>
#include <Rendering/Rendering.h>
#include <Rendering/TextureAtlas.h>

#include <Parsers/Parser.h>
#include <Parsers/TgxFile.h>

/* Gm1 container format for tiles/animations/images */
namespace Sourcehold
{
    namespace Parsers
    {
        using Rendering::TextureAtlas;

        class Gm1File : private Parser, public TextureAtlas
        {
                struct Gm1Header {
                    /* Unknown */
                    uint32_t u0[3];
                    /* Number of entries */
                    uint32_t num;
                    /* Unknown */
                    uint32_t u1;
                    /* Type of stored entries */
                    enum DataType : uint32_t {
                        TYPE_INTERFACE  = 0x01, /* Tgx images */
                        TYPE_ANIMATION  = 0x02, /* Animation */
                        TYPE_TILE       = 0x03, /* Tile object (tgx) */
                        TYPE_FONT       = 0x04, /* Tgx-compressed font */
                        TYPE_MISC1      = 0x05, /* Uncompressed image */
                        TYPE_CONSTSIZE  = 0x06, /* Constant size */
                        TYPE_MISC2      = 0x07, /* Like 0x05 */
                        FIRST           = TYPE_INTERFACE,
                        LAST            = TYPE_MISC2,
                    } type;
                    /* Unknown */
                    uint32_t u2[14];
                    /* Data size */
                    uint32_t len;
                    /* Unknown */
                    uint32_t u3;
                } header;

                struct ImageHeader {
                    /* Image dimensions */
                    uint16_t width;
                    uint16_t height;
                    /* Image offsets */
                    uint16_t offsetX;
                    uint16_t offsetY;
                    /* Unique image id */
                    uint16_t id;
                    /* Distance from top to bottom */
                    uint16_t dist;
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

                struct Gm1Entry {
                    ImageHeader header;
                    uint32_t size;
                    uint32_t offset;
                };

                std::string path;
                uint16_t palette[2560];

                /* Offset of data section */
                uint32_t offData;

                /* Raw image data */
                char *imgdata = nullptr;

                /* All of the entries */
                std::vector<Gm1Entry> entries;
            public:
                Gm1File(std::shared_ptr<Renderer> rend);
                ~Gm1File();

                bool LoadFromDisk(const std::string &path, bool threaded = true);
                void DumpInformation();

                inline std::vector<Gm1Entry>& GetEntries();
                inline Gm1Entry &GetEntry(uint32_t index);
            protected:
                const uint32_t max_num = 8192;
                bool GetImage(uint32_t index);
                void ReadPalette();
        };
    }
}
