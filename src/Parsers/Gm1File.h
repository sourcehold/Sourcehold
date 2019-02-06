#pragma once

#include <cinttypes>

#include <Config.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

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
                        TYPE_INTERFACE  = 0x01,
                        TYPE_ANIMATION  = 0x02,
                        TYPE_BUILDING   = 0x03,
                        TYPE_FONT       = 0x04,
                        TYPE_MISC1      = 0x05,
                        TYPE_MISC2      = 0x07
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
            
                Gm1File();
                ~Gm1File();

                bool LoadFromDisk(boost::filesystem::path path);
                void DumpInformation();
            protected:
                const uint32_t max_num = 255;
            private:
                Gm1Header header;
        };
    }
}
