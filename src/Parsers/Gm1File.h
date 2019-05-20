#pragma once

#include <cinttypes>
#include <memory>

#include <Rendering/Texture.h>
#include <Rendering/Tileset.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/Renderer.h>

#include <Parsers/Parser.h>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace Parsers
    {
        using namespace Rendering;

        /**
         * Container format for tiles/animations/images
         * Creates an entry in TextureAtlas for every image collection (Building, unit, etc.)
         * and a Tileset depending on the stored type
         */
        class Gm1File : private Parser
        {
        public:
            Gm1File(std::shared_ptr<Renderer> rend);
            Gm1File(std::shared_ptr<Renderer> rend, boost::filesystem::path path);
            Gm1File(const Gm1File&) = delete;
            Gm1File& operator=(const Gm1File&)= delete;
            ~Gm1File();

            bool LoadFromDisk(boost::filesystem::path path, bool threaded = false);
            void DumpInformation();
            void Free();

            inline std::weak_ptr<Tileset> GetTileset() { return tileset; }
            inline std::weak_ptr<TextureAtlas> GetTextureAtlas() { return textureAtlas; }
        protected:
            const uint32_t max_num = 2048;
            bool GetImage(uint32_t index);
            void ReadPalette();
        private:
            struct ImageHeader;
            struct Gm1Entry;
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

            std::shared_ptr<TextureAtlas> textureAtlas;
            std::shared_ptr<Tileset> tileset;
            std::shared_ptr<Renderer> renderer;
            boost::filesystem::path path;
            /* Color palette for tgx image entries */
            uint16_t palette[2560];
            /* Offset of data section */
            uint32_t offData;
            /* If tileset, how many actual entries does it contain? */
            uint32_t numCollections;
            /* Raw image data */
            char *imgdata = nullptr;
            /* All of the entries */
            std::vector<Gm1Entry> entries;
        };
    }
}
