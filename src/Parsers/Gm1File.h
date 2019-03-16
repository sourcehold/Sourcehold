#pragma once

#include <cinttypes>
#include <algorithm>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>

#include <System/Logger.h>

#include <Rendering/Texture.h>
#include <Rendering/Tileset.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/Renderer.h>
#include <Rendering/Rendering.h>

#include <Parsers/Parser.h>
#include <Parsers/TgxFile.h>

namespace Sourcehold
{
    namespace Parsers
    {
        using namespace Rendering;

        /*
         * Container format for tiles/animations/images
         *
         * Creates a texture in TextureAtlas for every image collection (Building, unit, etc.)
         */
        class Gm1File : private Parser, public TextureAtlas
        {
            public:
                Gm1File(std::shared_ptr<Renderer> rend);
                Gm1File(std::shared_ptr<Renderer> rend, const std::string &path);
                ~Gm1File();

                bool LoadFromDisk(const std::string &path, bool threaded = false);
                void DumpInformation();
                void Free();
            protected:
                const uint32_t max_num = 8192;
                bool GetCollections();
                bool GetImage(uint32_t index);
                void ReadPalette();
            private:
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
                struct ImageHeader;
                struct Gm1Entry;

                std::shared_ptr<Renderer> renderer;
                std::string path;
                /* Color palette for tgx image entries */
                uint16_t palette[2560];
                /* Offset of data section */
                uint32_t offData;
                uint32_t numCollections;
                /* Raw image data */
                char *imgdata = nullptr;
                /* All of the entries */
                std::vector<Gm1Entry> entries;
        };
    }
}
