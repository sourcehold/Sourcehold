#pragma once

#include <cinttypes>
#include <vector>

#include <Config.h>
#include <System/Logger.h>

#include <Rendering/Texture.h>
#include <Rendering/Rendering.h>

#include <Parsers/Parser.h>
#include <Parsers/TgxFile.h>

namespace Sourcehold
{
    namespace Parsers
    {
        class Gm1File : private Parser
        {
            public:
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
                    }type;
                    /* Unknown */
                    uint32_t u2[14];
                    /* Data size */
                    uint32_t len;
                    /* Unknown */
                    uint32_t u3;
                };

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
                    uint8_t direction;
                    /* Horizontal offset */
                    uint8_t horizOffset;
                    /* Width of building part */
                    uint8_t partWidth;
                    /* Color */
                    uint8_t color;
                };

                struct FileEntry {
                    ImageHeader header;
                    uint32_t size;
                    uint32_t offset;
                    Rendering::Texture image;
                };
            
                Gm1File();
                Gm1File(Rendering::Context &ctx);
                ~Gm1File();

                void SetContext(Rendering::Context &ctx);
                bool LoadFromDisk(std::string path);
                void DumpInformation();

                Rendering::Texture& GetImage(uint32_t n);
                uint32_t GetNumImages();
            private:
                Rendering::Context ctx;
                Gm1Header header;
                /* All of the images */
                std::vector<FileEntry> entries;
            protected:
                const uint32_t max_num = 1024;
                void ReadPalette();
        };
    }
}
