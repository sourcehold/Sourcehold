#pragma once

#include <cinttypes>

#include <Config.h>
#include <Parsers/Parser.h>
#include <System/Logger.h>
#include <Rendering/Texture.h>

using namespace OpenSH::Rendering;

namespace OpenSH
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

                bool LoadFromDisk(boost::filesystem::path path);
                void DumpInformation();
            protected:
                void ReadToken();
            private:
                TgxHeader header;
        };
    }
}
